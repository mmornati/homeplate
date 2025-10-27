# Getting Started with the New CI/CD System

Welcome! This guide will help you get started with the enhanced CI/CD system for HomePlate.

## 🎯 What's New?

Your HomePlate project now has a professional CI/CD system with:

- ✅ Automated builds for all ESP32 environments
- ✅ Unit test automation
- ✅ Pull request validation and auto-labeling
- ✅ Firmware size tracking
- ✅ Automated releases with release notes
- ✅ Dependency update monitoring
- ✅ Security scanning
- ✅ Comprehensive documentation

## 🚀 Quick Start

### 1. Enable GitHub Actions (if not already enabled)

1. Go to your repository on GitHub
2. Click on the **Actions** tab
3. If prompted, enable GitHub Actions

### 2. First Build

The CI will automatically run on your next push to main or when you create a PR.

To trigger manually:
1. Go to **Actions** → **Build and Test**
2. Click **Run workflow**
3. Select branch and click **Run workflow**

### 3. Create Your First Release

```bash
# Ensure you're on main and up to date
git checkout main
git pull

# Create a version tag
git tag v1.0.0

# Push the tag
git push origin v1.0.0

# Wait ~10 minutes for CI to complete
# Check Releases page for your new release!
```

## 📋 Daily Workflow

### Making Changes

```bash
# 1. Create a feature branch
git checkout -b feat/my-new-feature

# 2. Make your changes
# ... edit files ...

# 3. Copy config example (if needed)
cp src/config_example.h src/config.h

# 4. Test locally
pio test -e native -v
pio run -e inkplate10

# 5. Commit with conventional format
git commit -m "feat: add new feature"

# 6. Push and create PR
git push origin feat/my-new-feature
```

### Creating a Pull Request

1. Go to GitHub and create a PR
2. Fill out the PR template
3. Wait for CI checks to complete
4. Review the firmware size comparison
5. Address any issues
6. Get approval and merge

### Common Commit Types

```bash
git commit -m "feat: add new sleep schedule feature"
git commit -m "fix: resolve display refresh issue"
git commit -m "docs: update installation guide"
git commit -m "refactor: simplify MQTT code"
git commit -m "perf: optimize image rendering"
git commit -m "test: add battery level tests"
```

## 🎨 Labels

PRs are automatically labeled based on files changed:

| Changed Files | Auto Label |
|--------------|------------|
| `test/*` | tests |
| `.github/*` | ci/cd |
| `*.md` | documentation |
| `src/*.cpp`, `src/*.h` | code |
| `platformio.ini` | dependencies |

You can also manually add labels like `bug`, `enhancement`, `security`, etc.

## 📊 Understanding CI Checks

When you create a PR, you'll see several checks:

### ✅ Unit Tests
- Runs all unit tests in native environment
- Must pass for PR to be mergeable

### ✅ Build (inkplate10, inkplate10v2, debug)
- Builds firmware for each environment
- Ensures compatibility across devices

### ✅ Code Quality
- Runs static analysis
- Continues even if issues found

### ✅ PR Validation
- Checks PR title format
- Validates no config files committed
- Scans for potential secrets

### 📊 Firmware Size Check
- Compares firmware size to base branch
- Posts comment with comparison
- Helps catch unexpected increases

### 🔒 Security Check
- Scans for hardcoded secrets
- Blocks PR if secrets detected

## 🐛 Troubleshooting

### Build Fails: "config.h not found"

```bash
cp src/config_example.h src/config.h
```

### Build Fails: "library not found"

```bash
pio pkg install
pio run --target clean
pio run
```

### Test Fails

```bash
# Run tests with verbose output
pio test -e native -v

# Check the output for specific failures
# Fix the code or test
# Commit and push
```

### PR Blocked: "config.h detected"

- Never commit `config.h` (contains your personal settings)
- Remove it from your branch:
  ```bash
  git rm --cached src/config.h
  git commit -m "fix: remove config.h"
  git push
  ```

### Size Check Shows Large Increase

- Review what code was added
- Consider if the increase is justified
- Optimize if possible
- Comment on PR explaining the increase

## 📦 Dependency Management

### Weekly Updates

Every Monday, the CI checks for dependency updates and creates/updates an issue.

To update dependencies:

```bash
# Check for updates
pio pkg outdated

# Update all packages
pio pkg update

# Test the updates
pio test -e native -v
pio run -e inkplate10

# Commit
git commit -m "chore: update dependencies"
```

### Dependabot PRs

Dependabot will create PRs for GitHub Actions updates.

1. Review the PR
2. Check for breaking changes
3. Merge if CI passes

## 🎉 Release Process

### Creating a Release

```bash
# 1. Ensure main is clean and up to date
git checkout main
git pull origin main

# 2. Decide on version number (semantic versioning)
# v1.0.0 - Major (breaking changes)
# v1.1.0 - Minor (new features)
# v1.1.1 - Patch (bug fixes)

# 3. Create and push tag
git tag v1.0.0
git push origin v1.0.0

# 4. Wait for CI (about 10 minutes)
```

### What Happens Automatically

1. CI builds firmware for inkplate10 and inkplate10v2
2. CI generates release notes from PRs/commits
3. CI creates GitHub Release
4. CI attaches firmware binaries
5. CI adds installation instructions

### Release Notes

Release notes are automatically categorized:

- ✨ **New Features** - PRs/commits with "feat"
- 🐛 **Bug Fixes** - PRs/commits with "fix"
- 🔧 **Improvements** - PRs/commits with "refactor", "perf"
- 📝 **Other Changes** - Everything else

**Tip**: Use good PR titles and conventional commits for better release notes!

## 📚 Documentation

### Essential Reading

1. **[CI_QUICK_REFERENCE.md](.github/CI_QUICK_REFERENCE.md)** - Quick command reference
2. **[CI_CD_GUIDE.md](CI_CD_GUIDE.md)** - Complete CI/CD documentation
3. **[CONTRIBUTING.md](CONTRIBUTING.md)** - Contribution guidelines

### Templates

- **Pull Request Template** - Automatically shown when creating PRs
- **Bug Report Template** - Use when reporting bugs
- **Feature Request Template** - Use when suggesting features

## 🔍 Monitoring

### GitHub Actions Tab

- View all workflow runs
- Check build status
- Review logs
- Download artifacts

### Security Tab

- View CodeQL findings
- Check Dependabot alerts
- Review security advisories

### Insights Tab

- View contributor stats
- Check community activity
- See project pulse

## ✅ Best Practices

### Do's

- ✅ Use conventional commit format
- ✅ Write good PR descriptions
- ✅ Wait for CI before merging
- ✅ Review firmware size changes
- ✅ Test on actual hardware
- ✅ Keep PRs focused and small
- ✅ Update documentation
- ✅ Write unit tests for new features

### Don'ts

- ❌ Don't commit `config.h` or `config.cpp` with secrets
- ❌ Don't force push to main
- ❌ Don't merge failing PRs
- ❌ Don't ignore CI warnings
- ❌ Don't skip testing on hardware
- ❌ Don't create huge PRs
- ❌ Don't bypass security checks

## 🆘 Getting Help

### Resources

1. Check the [CI_CD_GUIDE.md](CI_CD_GUIDE.md)
2. Review workflow logs in Actions tab
3. Search existing issues
4. Check [PlatformIO docs](https://docs.platformio.org/)
5. Check [GitHub Actions docs](https://docs.github.com/en/actions)

### Creating an Issue

If you encounter CI/CD problems:

1. Use the bug report template
2. Include workflow run number
3. Attach relevant logs
4. Describe what you expected vs what happened
5. Add `ci/cd` label

## 🎓 Learning More

### Conventional Commits

Learn more: https://www.conventionalcommits.org/

Format: `type(scope): subject`

Types: feat, fix, docs, style, refactor, perf, test, build, ci, chore

### Semantic Versioning

Learn more: https://semver.org/

Format: `MAJOR.MINOR.PATCH`

- MAJOR: Breaking changes
- MINOR: New features (backward compatible)
- PATCH: Bug fixes

### GitHub Actions

Learn more: https://docs.github.com/en/actions

- Workflows
- Jobs and steps
- Matrix builds
- Artifacts and caching

## 🎊 Next Steps

Now that you understand the CI/CD system:

1. ✅ Create a test PR to see the system in action
2. ✅ Review the auto-generated labels
3. ✅ Check the firmware size comparison
4. ✅ Create a test release (optional)
5. ✅ Star your own repo for using such a great CI/CD system! 😄

## 💬 Feedback

Have suggestions for improving the CI/CD system?

1. Create a feature request issue
2. Label it with `ci/cd` and `enhancement`
3. Describe your idea
4. Explain the benefits

## 📅 Scheduled Activities

### Daily
- **Stale issue/PR check** (12am UTC)

### Weekly
- **Dependency updates** (Monday 9am UTC)
- **Security scan** (Monday 12am UTC)
- **Dependabot checks** (Monday)

### On Demand
- **Builds** - Every push and PR
- **Releases** - When you push tags
- **Manual workflows** - Via Actions tab

## 🎉 Congratulations!

You now have a world-class CI/CD system for your HomePlate project!

**Happy coding!** 🚀

---

*Need help? Check [CI_CD_GUIDE.md](CI_CD_GUIDE.md) or create an issue.*

