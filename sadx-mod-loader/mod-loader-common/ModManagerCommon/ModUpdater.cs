using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using IniFile;
using Newtonsoft.Json;

namespace ModManagerCommon
{
	public class ModUpdater
	{
		private readonly Dictionary<string, List<GitHubRelease>> gitHubCache = new Dictionary<string, List<GitHubRelease>>();
		public bool ForceUpdate;

		public ModDownload GetGitHubReleases(ModInfo mod, string folder,
			UpdaterWebClient client, List<string> errors)
		{
			List<GitHubRelease> releases;
			string url = "https://api.github.com/repos/" + mod.GitHubRepo + "/releases";

			if (!gitHubCache.ContainsKey(url))
			{
				try
				{
					string text = client.DownloadString(url);
					releases = JsonConvert.DeserializeObject<List<GitHubRelease>>(text)
						.Where(x => !x.Draft && !x.PreRelease)
						.ToList();

					if (releases.Count > 0)
					{
						gitHubCache[url] = releases;
					}
				}
				catch (Exception ex)
				{
					errors.Add($"[{mod.Name}] Error checking for updates at {url}: {ex.Message}");
					return null;
				}
			}
			else
			{
				releases = gitHubCache[url];
			}

			if (releases == null || releases.Count == 0)
			{
				// No releases available.
				return null;
			}

			string versionPath = Path.Combine("mods", folder, "mod.version");
			DateTime? localVersion = null;

			if (File.Exists(versionPath))
			{
				localVersion = DateTime.Parse(File.ReadAllText(versionPath).Trim());
			}
			else
			{
				var info = new FileInfo(Path.Combine("mods", folder, "mod.manifest"));
				if (info.Exists)
				{
					localVersion = info.LastWriteTimeUtc;
				}
			}

			GitHubRelease latestRelease = null;
			GitHubAsset latestAsset = null;

			foreach (GitHubRelease release in releases)
			{
				GitHubAsset asset = release.Assets
					.FirstOrDefault(x => x.Name.Equals(mod.GitHubAsset, StringComparison.OrdinalIgnoreCase));

				if (asset == null)
				{
					continue;
				}

				latestRelease = release;

				if (!ForceUpdate && localVersion.HasValue)
				{
					DateTime uploaded = DateTime.Parse(asset.Uploaded);

					if (localVersion >= uploaded)
					{
						// No updates available.
						break;
					}
				}

				latestAsset = asset;
				break;
			}

			if (latestRelease == null)
			{
				errors.Add($"[{mod.Name}] No releases with matching asset \"{mod.GitHubAsset}\" could be found in {releases.Count} release(s).");
				return null;
			}

			if (latestAsset == null)
			{
				return null;
			}

			string body = Regex.Replace(latestRelease.Body, "(?<!\r)\n", "\r\n");

			return new ModDownload(mod, Path.Combine("mods", folder), latestAsset.DownloadUrl, body, latestAsset.Size)
			{
				HomePage   = "https://github.com/" + mod.GitHubRepo,
				Name       = latestRelease.Name,
				Version    = latestRelease.TagName,
				Published  = latestRelease.Published,
				Updated    = latestAsset.Uploaded,
				ReleaseUrl = latestRelease.HtmlUrl
			};
		}

		public ModDownload GetGameBananaReleases(ModInfo mod, string folder,
			List<string> errors)
		{
			GameBananaItem gbi;
			try
			{
				gbi = GameBananaItem.Load(mod.GameBananaItemType, mod.GameBananaItemId.Value);
			}
			catch (Exception ex)
			{
				errors.Add($"[{mod.Name}] Error checking for updates: {ex.Message}");
				return null;
			}

			if (!gbi.HasUpdates)
			{
				// No releases available.
				return null;
			}

			string versionPath = Path.Combine("mods", folder, "mod.version");
			DateTime? localVersion = null;

			if (File.Exists(versionPath))
			{
				localVersion = DateTime.Parse(File.ReadAllText(versionPath).Trim());
			}
			else
			{
				var info = new FileInfo(Path.Combine("mods", folder, "mod.manifest"));
				if (info.Exists)
				{
					localVersion = info.LastWriteTimeUtc;
				}
			}

			GameBananaItemUpdate latestUpdate = gbi.Updates[0];

			if (!ForceUpdate && localVersion.HasValue)
			{
				if (localVersion >= latestUpdate.DateAdded)
				{
					// No updates available.
					return null;
				}
			}

			string body = string.Join(Environment.NewLine, latestUpdate.Changes.Select(a => a.Category + ": " + a.Text)) + Environment.NewLine + latestUpdate.Text;

			GameBananaItemFile dl = gbi.Files.First().Value;

			return new ModDownload(mod, Path.Combine("mods", folder), dl.DownloadUrl, body, dl.Filesize)
			{
				HomePage   = gbi.ProfileUrl,
				Name       = latestUpdate.Title,
				Version    = latestUpdate.Title,
				Published  = latestUpdate.DateAdded.ToString(CultureInfo.CurrentCulture),
				Updated    = latestUpdate.DateAdded.ToString(CultureInfo.CurrentCulture),
				ReleaseUrl = gbi.ProfileUrl
			};
		}

		public ModDownload CheckModularVersion(ModInfo mod, string folder, List<ModManifestEntry> localManifest,
			UpdaterWebClient client, List<string> errors)
		{
			if (!mod.UpdateUrl.StartsWith("http://", StringComparison.InvariantCulture)
				&& !mod.UpdateUrl.StartsWith("https://", StringComparison.InvariantCulture))
			{
				mod.UpdateUrl = "http://" + mod.UpdateUrl;
			}

			if (!mod.UpdateUrl.EndsWith("/", StringComparison.InvariantCulture))
			{
				mod.UpdateUrl += "/";
			}

			var url = new Uri(mod.UpdateUrl);
			url = new Uri(url, "mod.ini");

			ModInfo remoteInfo;

			try
			{
				Dictionary<string, Dictionary<string, string>> dict = IniFile.IniFile.Load(client.OpenRead(url));
				remoteInfo = IniSerializer.Deserialize<ModInfo>(dict);
			}
			catch (Exception ex)
			{
				errors.Add($"[{mod.Name}] Error pulling mod.ini from \"{mod.UpdateUrl}\": {ex.Message}");
				return null;
			}

			if (!ForceUpdate && remoteInfo.Version == mod.Version)
			{
				return null;
			}

			string manString;

			try
			{
				manString = client.DownloadString(new Uri(new Uri(mod.UpdateUrl), "mod.manifest"));
			}
			catch (Exception ex)
			{
				errors.Add($"[{mod.Name}] Error pulling mod.manifest from \"{mod.UpdateUrl}\": {ex.Message}");
				return null;
			}

			List<ModManifestEntry> remoteManifest;

			try
			{
				remoteManifest = ModManifest.FromString(manString);
			}
			catch (Exception ex)
			{
				errors.Add($"[{mod.Name}] Error parsing remote manifest from \"{mod.UpdateUrl}\": {ex.Message}");
				return null;
			}

			List<ModManifestDiff> diff = ModManifestGenerator.Diff(remoteManifest, localManifest);

			if (diff.Count < 1 || diff.All(x => x.State == ModManifestState.Unchanged))
			{
				return null;
			}

			string changes;

			if (!string.IsNullOrEmpty(mod.ChangelogUrl))
			{
				try
				{
					changes = client.DownloadString(new Uri(mod.ChangelogUrl));
				}
				catch (Exception ex)
				{
					changes = ex.Message;
				}
			}
			else
			{
				try
				{
					changes = client.DownloadString(new Uri(new Uri(mod.UpdateUrl), "changelog.txt"));
				}
				catch
				{
					// ignored
					changes = string.Empty;
				}
			}

			if (!string.IsNullOrEmpty(changes))
			{
				changes = Regex.Replace(changes, "(?<!\r)\n", "\r\n");
			}

			return new ModDownload(mod, Path.Combine("mods", folder), mod.UpdateUrl, changes, diff);
		}

		// TODO: cancel
		/// <summary>
		/// Get mod update metadata for the provided mods.
		/// </summary>
		/// <param name="updatableMods">Key-value pairs of mods to be checked, where the key is the mod path and the value is the mod metadata.</param>
		/// <param name="updates">Output list of mods with available updates.</param>
		/// <param name="errors">Output list of errors encountered during the update process.</param>
		public void GetModUpdates(List<KeyValuePair<string, ModInfo>> updatableMods,
			out List<ModDownload> updates, out List<string> errors, CancellationToken cancellationToken)
		{
			updates = new List<ModDownload>();
			errors = new List<string>();

			if (updatableMods == null || updatableMods.Count == 0)
			{
				return;
			}

			using (var client = new UpdaterWebClient())
			{
				foreach (KeyValuePair<string, ModInfo> info in updatableMods)
				{
					ModInfo mod = info.Value;
					if (!string.IsNullOrEmpty(mod.GitHubRepo))
					{
						if (string.IsNullOrEmpty(mod.GitHubAsset))
						{
							errors.Add($"[{mod.Name}] GitHubRepo specified, but GitHubAsset is missing.");
							continue;
						}

						ModDownload d = GetGitHubReleases(mod, info.Key, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
					{
						ModDownload d = GetGameBananaReleases(mod, info.Key, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.UpdateUrl))
					{
						List<ModManifestEntry> localManifest = null;
						string manPath = Path.Combine("mods", info.Key, "mod.manifest");

						if (!ForceUpdate && File.Exists(manPath))
						{
							try
							{
								localManifest = ModManifest.FromFile(manPath);
							}
							catch (Exception ex)
							{
								errors.Add($"[{mod.Name}] Error parsing local manifest: {ex.Message}");
								continue;
							}
						}

						ModDownload d = CheckModularVersion(mod, info.Key, localManifest, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
				}
			}
		}

		/// <summary>
		/// Clears update metadata cache.
		/// </summary>
		public void Clear()
		{
			gitHubCache.Clear();
		}
	}
}