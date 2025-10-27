# .github Directory

This directory contains GitHub-specific configuration files for the HomePlate project.

## 📁 Structure

```
.github/
├── workflows/              # GitHub Actions workflows
│   ├── build.yml          # Build and test
│   ├── pr-validation.yml  # PR validation
│   ├── release.yml        # Release management
│   ├── dependency-check.yml # Dependency updates
│   ├── codeql.yml         # Security analysis
│   ├── stale.yml          # Stale issue management
│   └── labels.yml         # Label synchronization
├── ISSUE_TEMPLATE/        # Issue templates
│   ├── bug_report.md      # Bug report template
│   └── feature_request.md # Feature request template
├── PULL_REQUEST_TEMPLATE.md # PR template
├── dependabot.yml         # Dependabot configuration
├── labels.yml             # Label definitions
├── CI_QUICK_REFERENCE.md  # Quick reference guide
└── README.md              # This file
```

## 🔄 Workflows

### [build.yml](workflows/build.yml)
**Main build and test workflow**

- Triggers: Push, PR, manual
- Jobs: Unit tests, matrix builds, code quality, CI summary
- Environments: inkplate10, inkplate10v2, debug, native
- Artifacts: Firmware binaries, test results

### [pr-validation.yml](workflows/pr-validation.yml)
**Pull request validation**

- Triggers: PR events (non-draft)
- Jobs: Validation, size check, security scan
- Features: Auto-labeling, size comparison, secret detection

### [release.yml](workflows/release.yml)
**Automated release management**

- Triggers: Version tags (v*.*.*)
- Jobs: Build binaries, create release
- Features: Auto-generated release notes, binary attachments

### [dependency-check.yml](workflows/dependency-check.yml)
**Dependency update monitoring**

- Triggers: Weekly (Monday 9am UTC)
- Jobs: Check PlatformIO packages
- Features: Creates/updates GitHub issues

### [codeql.yml](workflows/codeql.yml)
**Security analysis**

- Triggers: Push, PR, weekly (Monday 12am UTC)
- Jobs: CodeQL analysis for C++
- Features: Security vulnerability detection

### [stale.yml](workflows/stale.yml)
**Stale issue and PR management**

- Triggers: Daily (12am UTC)
- Jobs: Mark and close stale items
- Features: Auto-remove on activity

### [labels.yml](workflows/labels.yml)
**Label synchronization**

- Triggers: Push to main (when labels.yml changes), manual
- Jobs: Sync repository labels
- Features: Consistent labeling

## 📝 Templates

### [PULL_REQUEST_TEMPLATE.md](PULL_REQUEST_TEMPLATE.md)
Template for pull requests with:
- Description and change type
- Testing checklist
- Quality checklist
- Related issues

### [ISSUE_TEMPLATE/bug_report.md](ISSUE_TEMPLATE/bug_report.md)
Template for bug reports with:
- Hardware information
- Reproduction steps
- Configuration details
- Serial output

### [ISSUE_TEMPLATE/feature_request.md](ISSUE_TEMPLATE/feature_request.md)
Template for feature requests with:
- Problem statement
- Proposed solution
- Use cases
- Implementation ideas

## ⚙️ Configuration

### [dependabot.yml](dependabot.yml)
Configures Dependabot to:
- Check GitHub Actions weekly
- Auto-assign to maintainer
- Label with `dependencies` and `github-actions`

### [labels.yml](labels.yml)
Defines repository labels:
- Types: bug, enhancement, documentation
- Areas: code, tests, library, ci/cd
- Status: stale, work-in-progress, pinned
- Priority: security, performance, help wanted

## 📚 Documentation

### [CI_QUICK_REFERENCE.md](CI_QUICK_REFERENCE.md)
Quick reference for:
- Workflow triggers
- Common commands
- Commit types
- Troubleshooting

## 🎯 Quick Links

- [Full CI/CD Guide](../CI_CD_GUIDE.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
- [CI/CD Improvements](../CI_CD_IMPROVEMENTS.md)

## 🔍 Viewing Workflow Runs

1. Go to the [Actions tab](../../actions) on GitHub
2. Select a workflow from the sidebar
3. Click on a specific run to view details
4. Click on a job to view logs

## 🛠️ Modifying Workflows

When modifying workflows:

1. Test changes in a feature branch
2. Use workflow syntax checker
3. Check for YAML syntax errors
4. Test with workflow_dispatch trigger
5. Review logs carefully
6. Document changes

## 🆘 Troubleshooting

### Workflow Not Triggering
- Check trigger conditions
- Verify branch names
- Check file paths (for path-specific triggers)
- Ensure workflow file is in `workflows/` directory

### Permission Errors
- Check `permissions:` in workflow
- Verify token scopes
- Check repository settings

### Cache Issues
- Cache keys might need updating
- Clear cache via Actions UI
- Check cache size limits

## 📖 Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Workflow Syntax Reference](https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions)
- [PlatformIO CI/CD](https://docs.platformio.org/en/latest/integration/ci/index.html)

## 🤝 Contributing

When contributing to CI/CD:

1. Read [CONTRIBUTING.md](../CONTRIBUTING.md)
2. Test changes thoroughly
3. Document new workflows
4. Update this README if needed
5. Follow best practices

---

*For detailed information, see [CI_CD_GUIDE.md](../CI_CD_GUIDE.md)*

