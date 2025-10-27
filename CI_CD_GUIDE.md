# CI/CD Guide for HomePlate

This document describes the comprehensive CI/CD setup for the HomePlate project.

## Table of Contents

- [Overview](#overview)
- [Workflows](#workflows)
  - [Build and Test](#build-and-test)
  - [Pull Request Validation](#pull-request-validation)
  - [Dependency Updates](#dependency-updates)
  - [Release Management](#release-management)
  - [Security Analysis](#security-analysis)
  - [Stale Issues and PRs](#stale-issues-and-prs)
- [Best Practices](#best-practices)
- [Creating a Release](#creating-a-release)
- [Troubleshooting](#troubleshooting)

## Overview

The HomePlate project uses GitHub Actions for continuous integration and deployment. The CI/CD pipeline includes:

- ✅ Automated building for all ESP32 environments
- ✅ Unit testing with native environment
- ✅ Code quality and security analysis
- ✅ Firmware size comparison on PRs
- ✅ Automated dependency updates
- ✅ Automatic release creation with release notes
- ✅ PR validation and labeling

## Workflows

### Build and Test

**File**: `.github/workflows/build.yml`

**Triggers**:
- Push to `main` branch
- Push to `bugfix/*` and `feature/*` branches
- Pull requests to `main`
- Manual dispatch

**Jobs**:

1. **Unit Tests** (`test`)
   - Runs unit tests using the `native` PlatformIO environment
   - Uploads test results as artifacts
   - Runs first to fail fast if tests don't pass

2. **Build Matrix** (`build`)
   - Builds firmware for multiple environments:
     - `inkplate10` - For Inkplate 10 devices
     - `inkplate10v2` - For Inkplate 10 v2 devices
     - `debug` - Debug build with verbose logging
   - Uploads firmware binaries as artifacts (retained for 30 days)
   - Checks and reports firmware size
   - Fails fast disabled to build all environments even if one fails

3. **Code Quality** (`check`)
   - Runs static analysis with cppcheck
   - Continues even if issues are found (non-blocking)

4. **CI Success** (`ci-success`)
   - Summary job that checks if all required jobs passed
   - Useful for branch protection rules

**Caching Strategy**:
- Python pip packages cached by requirements.txt hash
- PlatformIO packages cached by platformio.ini hash
- Significantly reduces build times

### Pull Request Validation

**File**: `.github/workflows/pr-validation.yml`

**Triggers**:
- Pull request opened, synchronized, reopened, or marked ready for review
- Only runs on non-draft PRs

**Jobs**:

1. **PR Validation** (`validate`)
   - Checks PR title follows conventional commits (feat, fix, docs, etc.)
   - Ensures `config.h` is not committed (security check)
   - Warns if `config.cpp` is committed
   - Validates `platformio.ini` syntax
   - Auto-labels PR based on changed files:
     - `tests` - Changes in test/
     - `ci/cd` - Changes in .github/
     - `documentation` - Changes to .md files
     - `code` - Changes to .cpp/.h files
     - `dependencies` - Changes to platformio.ini
     - `library` - Changes in lib/

2. **Firmware Size Check** (`size-check`)
   - Builds firmware for both PR and base branch
   - Compares firmware sizes
   - Posts detailed comparison comment on PR
   - Updates comment on subsequent pushes
   - Helps catch unexpected size increases

3. **Security Check** (`security-check`)
   - Scans for potential secrets in code
   - Looks for hardcoded passwords, API keys, tokens
   - Fails PR if potential secrets are detected

### Dependency Updates

**File**: `.github/workflows/dependency-check.yml`

**Triggers**:
- Weekly schedule (Monday at 9:00 AM UTC)
- Manual dispatch

**Jobs**:

1. **Check Dependencies** (`check-dependencies`)
   - Checks for PlatformIO core updates
   - Checks for library dependency updates
   - Checks for platform package updates
   - Generates comprehensive update report
   - Creates or updates GitHub issue with outdated dependencies
   - Issue is labeled with `dependencies` and `automation`

**Configuration**: `.github/dependabot.yml`
- Automatically checks for GitHub Actions updates
- Creates PRs for action version updates weekly
- Note: PlatformIO dependencies checked via custom workflow

### Release Management

**File**: `.github/workflows/release.yml`

**Triggers**:
- Push of version tags (e.g., `v1.0.0`, `v2.1.3`)
- Manual dispatch with tag input

**Jobs**:

1. **Build Release Binaries** (`build-release`)
   - Matrix builds for production environments:
     - `inkplate10`
     - `inkplate10v2`
   - Names binaries with version: `homeplate-v1.0.0-inkplate10.bin`
   - Uploads binaries as artifacts (retained for 90 days)

2. **Create GitHub Release** (`create-release`)
   - Generates comprehensive release notes:
     - ✨ New Features
     - 🐛 Bug Fixes
     - 🔧 Improvements
     - 📝 Other Changes
   - Categorizes changes from:
     - Merged pull requests (preferred)
     - Commit messages (fallback)
   - Includes installation instructions
   - Links to full changelog
   - Attaches firmware binaries
   - Creates release automatically (not draft)

**Release Notes Logic**:
- Compares current tag with previous tag
- Extracts merged PRs between tags
- Categorizes by PR labels and titles
- Falls back to commit messages if no PRs
- Provides installation commands

### Security Analysis

**File**: `.github/workflows/codeql.yml`

**Triggers**:
- Push to `main`
- Pull requests to `main`
- Weekly schedule (Monday at midnight UTC)
- Manual dispatch

**Jobs**:

1. **CodeQL Analysis** (`analyze`)
   - Analyzes C++ code for security vulnerabilities
   - Uses security-and-quality query suite
   - Builds project to analyze compiled code
   - Reports findings to GitHub Security tab
   - Helps identify:
     - Memory safety issues
     - Buffer overflows
     - Use-after-free bugs
     - SQL injection (if applicable)
     - And more

### Stale Issues and PRs

**File**: `.github/workflows/stale.yml`

**Triggers**:
- Daily schedule at midnight UTC
- Manual dispatch

**Configuration**:

**Issues**:
- Marked stale after 60 days of inactivity
- Closed after 14 additional days if still inactive
- Exempt labels: `pinned`, `security`, `bug`, `enhancement`, `help-wanted`

**Pull Requests**:
- Marked stale after 30 days of inactivity
- Closed after 14 additional days if still inactive
- Exempt labels: `pinned`, `security`, `work-in-progress`

**Behavior**:
- Automatically removes stale label when updated
- Posts friendly messages when marking stale or closing
- Helps keep repository tidy

### Label Management

**File**: `.github/workflows/labels.yml`

**Configuration**: `.github/labels.yml`

**Purpose**:
- Syncs repository labels with configuration file
- Ensures consistent labeling across project
- Automatically creates missing labels
- Does not delete existing labels

**Available Labels**:
- `bug`, `enhancement`, `documentation`
- `dependencies`, `github-actions`, `ci/cd`
- `code`, `tests`, `library`
- `security`, `performance`, `refactoring`
- `hardware`, `good first issue`, `help wanted`
- And more...

## Best Practices

### Pull Requests

1. **Use Conventional Commit format for PR titles**:
   - `feat: Add new sleep schedule feature`
   - `fix: Resolve touchpad sensitivity issue`
   - `docs: Update installation instructions`
   - `refactor: Improve MQTT connection handling`
   - `perf: Optimize image rendering`
   - `test: Add tests for sleep duration calculation`

2. **Keep PRs focused**:
   - One feature or fix per PR
   - Easier to review and test
   - Simpler to revert if needed

3. **Never commit `config.h`**:
   - Contains personal configuration and potentially secrets
   - Use `config_example.h` as template
   - CI will automatically fail if detected

4. **Check firmware size**:
   - Review the size comparison comment
   - Investigate unexpected increases
   - ESP32 has limited flash memory

5. **Wait for CI to pass**:
   - All checks must pass before merging
   - Review any warnings or errors
   - Fix issues in the PR branch

### Releases

1. **Follow Semantic Versioning**:
   - `vMAJOR.MINOR.PATCH` (e.g., `v1.2.3`)
   - MAJOR: Breaking changes
   - MINOR: New features (backward compatible)
   - PATCH: Bug fixes

2. **Tag from main branch**:
   ```bash
   git checkout main
   git pull
   git tag v1.0.0
   git push origin v1.0.0
   ```

3. **Write good commit messages**:
   - They appear in release notes
   - Follow conventional commits format
   - Be descriptive but concise

4. **Merge PRs with meaningful titles**:
   - PR titles become release note entries
   - Use conventional commit format
   - Describe the change clearly

### Dependencies

1. **Keep dependencies updated**:
   - Review weekly dependency check issues
   - Test updates locally before merging
   - Check for breaking changes in release notes

2. **Pin critical dependencies**:
   - Use exact versions for stability: `1.2.3`
   - Use caret for minor updates: `^1.2.3`
   - Use tilde for patch updates: `~1.2.3`

3. **Review Dependabot PRs**:
   - Check for breaking changes
   - Review changelog of updated dependency
   - Run CI tests before merging

### Testing

1. **Write unit tests for new features**:
   - Use the `native` environment
   - Follow existing test patterns
   - Test edge cases and error conditions

2. **Test on actual hardware**:
   - CI can't test hardware functionality
   - Test touchpads, display, battery, etc.
   - Verify sleep modes work correctly

3. **Test OTA updates**:
   - Ensure OTA still works after changes
   - Test update from previous version
   - Verify rollback if needed

## Creating a Release

### Method 1: Using Git Tags (Recommended)

1. Ensure all changes are merged to `main`:
   ```bash
   git checkout main
   git pull origin main
   ```

2. Create and push a version tag:
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

3. The release workflow will automatically:
   - Build firmware binaries
   - Generate release notes
   - Create GitHub release
   - Attach binaries to release

### Method 2: Using GitHub Actions Manually

1. Go to Actions → Release workflow
2. Click "Run workflow"
3. Enter the version tag (e.g., `v1.0.0`)
4. Click "Run workflow"

### After Release

1. **Verify the release**:
   - Check GitHub Releases page
   - Download and test binaries
   - Review release notes for accuracy

2. **Announce the release**:
   - Update discussions or forums
   - Notify users of new features
   - Provide upgrade instructions if needed

3. **Monitor for issues**:
   - Watch for bug reports
   - Check error logs
   - Be ready to release hotfix if needed

## Troubleshooting

### Build Failures

**Problem**: Build fails with "library not found"

**Solution**: 
```bash
# Clean and reinstall dependencies
pio pkg install
pio run --target clean
pio run
```

**Problem**: Build fails with "config.h not found"

**Solution**:
```bash
# Copy example config
cp src/config_example.h src/config.h
```

### Test Failures

**Problem**: Tests fail locally but pass in CI

**Solution**:
- Ensure you're using the same PlatformIO version
- Check for local config differences
- Run tests in clean environment:
  ```bash
  pio test -e native -v
  ```

**Problem**: Tests fail in CI but pass locally

**Solution**:
- Check CI logs for specific errors
- Ensure all test dependencies are in platformio.ini
- Test with fresh PlatformIO installation

### Release Issues

**Problem**: Release workflow doesn't trigger on tag push

**Solution**:
- Ensure tag follows `v*.*.*` format
- Check tag was pushed: `git push origin --tags`
- Verify GitHub Actions permissions

**Problem**: Release notes are missing PRs

**Solution**:
- Ensure PRs are merged, not force-pushed
- Check PR merge commit is in tag history
- Use conventional commit format in PR titles

### Size Check Issues

**Problem**: Size check shows large unexpected increase

**Solution**:
- Review what code was added
- Check for accidentally included files
- Look for debug symbols or verbose logging
- Consider optimizing or removing features
- Check compiler optimization flags

### Permission Errors

**Problem**: Workflow fails with permission error

**Solution**:
- Check workflow has necessary permissions
- Verify `GITHUB_TOKEN` has correct scopes
- For PR workflows, check fork PR restrictions

## Additional Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [Semantic Versioning](https://semver.org/)

## Questions or Issues?

If you encounter issues with CI/CD workflows:

1. Check this documentation first
2. Review workflow logs in GitHub Actions
3. Search existing issues
4. Create a new issue with:
   - Workflow name and run number
   - Error messages or logs
   - Steps to reproduce
   - Expected vs actual behavior

