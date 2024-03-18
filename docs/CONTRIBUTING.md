# How to contribute? {#contributing}

This project is another abstraction layer above at least 4 other projects (like the WAMP, MEAN and other popular stacks for the web). 
This essentially means that this repostiory might need updates to reflect changes and improvements in the other libraries but also that some contributions are better targeted at the other repositories.

## Open an issue first

The best thing to do when in doubt would be to either open a [discussion](https://github.com/Paper-Cranes-Ltd/big2-stack/discussions) or an [issue](https://github.com/Paper-Cranes-Ltd/big2-stack/issues).
This way someone can take a look at it and decide if it is something we need to fix in this repository or the third-party ones (or even create a workaround in this one).
I have applied fixes to some of these repositories previously and there is also the patch approach which can help fix issues.

## Submit a pull request

If you have an idea how to fix an issue or it was already discussed please do submit a pull request.
Keep in mind that pull requests should be relatively small and on point.
Keep changes contained and simple as much as you can on the topic of the change or even broken down to small incremental steps.

### How to commit?

When creating commits please use the [conventional commits](https://www.conventionalcommits.org/en/v1.0.0/) style since I generate the changelist based on those.
I use mostly **feat** for any new features, **fix** for bugfixes and **chore** for anything tha shouldn't go into the changelist.
Inside the description use the #&lt;issue-number&gt; to link an issue and "Fixes #&lt;issue-number&gt;" or "Resolves #&lt;issue-number&gt;" to resolve an issue automatically with the pull request approval.

## Writing documentation

This project has a generated documentation approach where classes, functions and markdown files are combined using doxygen and published onto the github pages platform.
If you find code documentation lacking some explanation please do create a pull request with your proposed changes. This is also valid for the markdown pages in the docs/ folder.
