import re
from itertools import takewhile
from git import Repo, Commit, Tag
from pathlib import Path
from re import match

cwd = Path.cwd()

repo = Repo(cwd)

if __name__ == "__main__":
    tags: list[Tag] = list(repo.tags)
    tags.reverse()
    commits: list[Commit] = list(repo.iter_commits())
    result = []

    def tag_is_reached(commit: Commit):
        return tags[0].commit.hexsha != commit.hexsha

    versions = {}
    current_tag_name = "Current"
    while len(tags) > 0:
        current_tag_commits = list(takewhile(tag_is_reached, commits))
        versions[current_tag_name] = current_tag_commits
        current_tag_name = tags[0].name
        tags = tags[1:]
        commits = commits[len(current_tag_commits):]

    versions[current_tag_name] = commits

    lines = []
    lines.append("# Changelist {#changelist}")
    lines.append("")
    lines.append("*This changelist is generated thanks to [Conventional Commits](https://www.conventionalcommits.org/)*")
    lines.append("[TOC]")
    lines.append("")

    for version in versions:
        lines.append(f"## {version}")
        lines.append("")

        for commit in versions[version]:

            fix_match = match(r'fix(?P<feature>\(.*\))?!?: (?P<summary>.*)', commit.summary)
            if fix_match:
                intro = "Fix"
                if fix_match['feature']:
                    intro += f"[{fix_match['feature']}]"

                lines.append(f"- {intro}: **{fix_match['summary']}**")

            feat_match = match(r'feat(?P<feature>\(.*\))?!?: (?P<summary>.*)', commit.summary)
            if feat_match:
                intro = "Feature"
                if feat_match['feature']:
                    intro += f"[{feat_match['feature']}]"

                lines.append(f"- {intro}: **{feat_match['summary']}**")

        if lines[-1] == "":
            lines.append("- *No Major Changes*")

        lines.append("")

    markdown_path = Path(cwd) / "docs" / "changelist.md"
    markdown_path.write_text("\n".join(lines))

