import json
import os
import subprocess
import sys
import urllib.request
import pathlib

kinocheckURL = "https://api.kinocheck.com/trailers/latest?language=en"
youtubeBaseURL = "https://youtube.com/watch?v="

kinocheckSeenTrailersFile: pathlib.Path = (
    pathlib.Path.home()
    /
    ".local/kinocheck/seen-trailers.txt"
)
kinocheckSeenTrailersFile.parent.mkdir(parents=True, exist_ok=True)

#ydlScript: pathlib.Path = pathlib.Path.home() / "ydl.sh"

saveTrailersTo = pathlib.Path.home() / "downloads/trailers/"
saveTrailersTo.mkdir(parents=False, exist_ok=True)


def getYouTubeVideoIDs() -> list[str]:
    data: dict = {}

    request = urllib.request.Request(
        kinocheckURL,
        # 403 without setting `User-Agent` header
        headers={"User-Agent": "yt-dlp"}
    )
    with urllib.request.urlopen(request, timeout=15) as response:
        data = json.load(response)

    # aside from numeric nodes with `youtube_video_id` properties, there is also a useless
    # `_metadata` node, which should be skipped
    nodes = data.values() if isinstance(data, dict) else data
    return [
        node["youtube_video_id"]
        for node in nodes
        if isinstance(node, dict) and node.get("youtube_video_id")
    ]


def runYtdlp(youtubeVideoID) -> int:
    youtubeVideoURL: str = f"{youtubeBaseURL}{youtubeVideoID}"
    print(f"Downloading {youtubeVideoURL}")

    ytDlpResult: int = 0
    try:
        result = subprocess.run(
            #[
            #    str(ydlScript),
            #    youtubeVideoURL
            #],
            # this is mostly just to change the download path, otherwise trailers downloads
            # should probably go through the same main script
            [
                "/usr/bin/yt-dlp",
                "-P",
                saveTrailersTo,
                youtubeVideoURL
            ],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
        ytDlpResult = result.returncode
    except OSError as ex:
        print(f"Failed to run yt-dlp: {ex}", file=sys.stderr)
        ytDlpResult = 1

    return ytDlpResult


def main() -> int:
    try:
        youtubeVideoIDs = getYouTubeVideoIDs()
    except (OSError, ValueError) as ex:
        print(f"Failed to get trailers: {ex}", file=sys.stderr)
        return 1

    seenTrailers: set[str] = set()
    if kinocheckSeenTrailersFile.is_file():
        try:
            trailersFileContents = kinocheckSeenTrailersFile.read_text()
            seenTrailers = {line.strip() for line in trailersFileContents.splitlines() if line.strip()}
        except FileNotFoundError:
            print(
                "Could not get a list of seen trailers from the file",
                file=sys.stderr
            )

    newTrailers = []
    for yvi in youtubeVideoIDs:
        if yvi not in seenTrailers:
            newTrailers.append(yvi)
            seenTrailers.add(yvi)

    if newTrailers:
        for trailer in newTrailers:
            ytdlpResult: int = runYtdlp(trailer)
            if ytdlpResult == 0:
                with kinocheckSeenTrailersFile.open("a") as f:
                    f.write(trailer + "\n")
            else:
                print(
                    f"yt-dlp exited with {ytdlpResult} for {trailer}",
                    file=sys.stderr
                )
    else:
        print("No new trailers")

    return 0


if __name__ == "__main__":
    sys.exit(main())
