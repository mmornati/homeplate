# CI/CD Quick Reference

Quick reference guide for HomePlate CI/CD workflows.

## 📋 Workflow Triggers

| Workflow | Trigger | Purpose |
|----------|---------|---------|
| **Build and Test** | Push to main/bugfix/feature, PRs | Build firmware and run tests |
| **PR Validation** | Pull requests | Validate PR quality and size |
| **Release** | Version tags (`v*.*.*`) | Create releases with binaries |
| **Dependency Check** | Weekly (Mon 9am) | Check for outdated packages |
| **CodeQL** | Push, PR, Weekly (Mon 12am) | Security analysis |
| **Stale** | Daily (12am) | Manage stale issues/PRs |

## 🚀 Quick Commands

### Local Development
```bash
# Build
pio run -e inkplate10

# Test
pio test -e native -v

# Clean
pio run --target clean

# Check code
pio check -e inkplate10

# Update dependencies
pio pkg update

# OTA update
pio run -e ota -t upload
```

### Git Workflow
```bash
# Create feature branch
git checkout -b feat/your-feature

# Commit with conventional format
git commit -m "feat: add new feature"

# Push and create PR
git push origin feat/your-feature

# Update from main
git fetch upstream
git rebase upstream/main
```

## 📝 Commit Types

| Type | Description | Example |
|------|-------------|---------|
| `feat` | New feature | `feat: add MQTT reconnect logic` |
| `fix` | Bug fix | `fix: resolve display refresh issue` |
| `docs` | Documentation | `docs: update API documentation` |
| `refactor` | Code refactoring | `refactor: simplify network code` |
| `perf` | Performance | `perf: optimize image decoding` |
| `test` | Tests | `test: add sleep duration tests` |
| `ci` | CI/CD | `ci: add dependency checking` |
| `chore` | Maintenance | `chore: update dependencies` |

## 🏷️ Auto Labels

PRs are automatically labeled based on files changed:

| Files Changed | Label |
|--------------|-------|
| `test/*` | tests |
| `.github/*` | ci/cd |
| `*.md` | documentation |
| `src/*.cpp`, `src/*.h` | code |
| `platformio.ini` | dependencies |
| `lib/*` | library |

## ✅ PR Checklist

Before submitting a PR:

- [ ] Follows conventional commit format
- [ ] No `config.h` or secrets committed
- [ ] Unit tests pass locally
- [ ] Builds for all environments
- [ ] Firmware size acceptable
- [ ] Tested on hardware (if applicable)
- [ ] Documentation updated
- [ ] Branch up to date with main

## 🎉 Creating a Release

### Quick Steps
```bash
# 1. Ensure main is up to date
git checkout main
git pull origin main

# 2. Create and push tag
git tag v1.2.3
git push origin v1.2.3

# 3. Wait for CI to create release
# 4. Verify release on GitHub
```

### Version Format
- `v1.0.0` - Major release (breaking changes)
- `v1.1.0` - Minor release (new features)
- `v1.1.1` - Patch release (bug fixes)

## 🔍 Debugging CI Failures

### Build Failure
1. Check error in CI logs
2. Reproduce locally: `pio run -e inkplate10`
3. Check dependencies: `pio pkg install`
4. Clean build: `pio run --target clean`

### Test Failure
1. Run locally: `pio test -e native -v`
2. Check test output
3. Fix code or test
4. Push update

### Size Check Warning
1. Review size comparison comment
2. Check what changed
3. Optimize if needed
4. Document if intentional

### Security Check Failure
1. Review security scan results
2. Never commit secrets
3. Use config files
4. Update code

## 📊 CI Status Badges

Add to README:

```markdown
![Build Status](https://github.com/mmornati/homeplate/workflows/Build%20and%20Test/badge.svg)
![Release](https://github.com/mmornati/homeplate/workflows/Release/badge.svg)
![CodeQL](https://github.com/mmornati/homeplate/workflows/CodeQL%20Security%20Analysis/badge.svg)
```

## 🔧 Environment Matrix

| Environment | Platform | Purpose |
|------------|----------|---------|
| `inkplate10` | ESP32 | Production (Inkplate 10) |
| `inkplate10v2` | ESP32 | Production (Inkplate 10 v2) |
| `debug` | ESP32 | Debug builds |
| `native` | Native | Unit tests |
| `ota` | ESP32 | OTA updates |

## 📦 Dependencies

### Managed by Dependabot
- GitHub Actions (weekly)

### Managed by Dependency Check
- PlatformIO core
- PlatformIO libraries
- ESP32 platform

### Manual
- PlatformIO libraries in `platformio.ini`
  ```bash
  # Check for updates
  pio pkg outdated
  
  # Update all
  pio pkg update
  ```

## ⚡ CI Performance

### Caching
- Python packages (~30s saved)
- PlatformIO packages (~2min saved)
- Cache key: `platformio.ini` hash

### Optimization Tips
- Use matrix builds for parallel execution
- Cache dependencies
- Skip unnecessary steps with conditionals
- Use `fail-fast: false` for complete results

## 🐛 Common Issues

### "Config file not found"
```bash
cp src/config_example.h src/config.h
```

### "Library not found"
```bash
pio pkg install
```

### "Permission denied"
```bash
# Linux/Mac
sudo usermod -a -G dialout $USER
# Then logout/login
```

### "Device not found"
```bash
pio device list
# Check USB connection
```

## 📚 More Information

- Full guide: [CI_CD_GUIDE.md](../CI_CD_GUIDE.md)
- Contributing: [CONTRIBUTING.md](../CONTRIBUTING.md)
- PlatformIO docs: https://docs.platformio.org/
- GitHub Actions docs: https://docs.github.com/actions

## 🆘 Need Help?

1. Check workflow logs in GitHub Actions tab
2. Search existing issues
3. Read full documentation
4. Create a discussion or issue

---

*Last updated: 2025-10-27*

