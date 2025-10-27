# Contributing to HomePlate

Thank you for considering contributing to HomePlate! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [CI/CD](#cicd)

## Code of Conduct

This project follows a simple code of conduct:

- Be respectful and inclusive
- Be patient with newcomers
- Focus on what is best for the community
- Show empathy towards other community members

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- ESP32 development board (Inkplate 10 or 10v2)
- USB cable for flashing
- Basic knowledge of C++ and Arduino framework

### Setting Up Your Development Environment

1. **Fork the repository**
   ```bash
   # Click "Fork" on GitHub
   ```

2. **Clone your fork**
   ```bash
   git clone https://github.com/YOUR_USERNAME/homeplate.git
   cd homeplate
   ```

3. **Add upstream remote**
   ```bash
   git remote add upstream https://github.com/mmornati/homeplate.git
   ```

4. **Create configuration file**
   ```bash
   cp src/config_example.h src/config.h
   # Edit config.h with your settings
   ```

5. **Install dependencies**
   ```bash
   pio pkg install
   ```

6. **Build the project**
   ```bash
   pio run
   ```

7. **Run tests**
   ```bash
   pio test -e native
   ```

## Development Workflow

### Creating a Feature Branch

Always create a new branch for your work:

```bash
# Update your main branch
git checkout main
git pull upstream main

# Create a feature branch
git checkout -b feature/your-feature-name
# or
git checkout -b bugfix/issue-description
```

### Making Changes

1. **Make your changes** in small, logical commits
2. **Test your changes** on actual hardware if applicable
3. **Run unit tests** to ensure nothing broke
4. **Check code quality** using static analysis

```bash
# Run tests
pio test -e native -v

# Build for your device
pio run -e inkplate10

# Check for issues
pio check -e inkplate10
```

### Commit Messages

Follow the [Conventional Commits](https://www.conventionalcommits.org/) format:

```
type(scope): subject

body (optional)

footer (optional)
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `build`: Build system changes
- `ci`: CI/CD changes
- `chore`: Other changes (dependencies, etc.)

**Examples**:
```bash
git commit -m "feat: add support for custom sleep schedules"
git commit -m "fix: resolve touchpad sensitivity issues on v2 devices"
git commit -m "docs: update installation instructions for OTA"
git commit -m "refactor: improve MQTT connection handling"
```

### Keeping Your Branch Updated

Regularly sync your branch with upstream:

```bash
git fetch upstream
git rebase upstream/main
```

If there are conflicts, resolve them and continue:

```bash
# Fix conflicts in your editor
git add .
git rebase --continue
```

## Pull Request Process

### Before Submitting

- [ ] All tests pass locally
- [ ] Code follows project style guidelines
- [ ] Documentation is updated if needed
- [ ] Commit messages follow conventional format
- [ ] No `config.h` or sensitive data included
- [ ] Branch is up to date with main
- [ ] Changes are tested on actual hardware (if applicable)

### Submitting a Pull Request

1. **Push your branch**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create Pull Request on GitHub**
   - Go to your fork on GitHub
   - Click "New Pull Request"
   - Fill out the PR template
   - Link any related issues

3. **Wait for CI checks**
   - All CI checks must pass
   - Review any failures and fix them

4. **Address Review Comments**
   - Make requested changes
   - Push new commits to your branch
   - Respond to comments

5. **Squash commits if requested**
   ```bash
   git rebase -i HEAD~n  # where n is number of commits
   git push --force-with-lease
   ```

### PR Requirements

- **Title**: Must follow conventional commit format
  - Example: `feat: add new sleep schedule feature`
- **Description**: Use the PR template
- **Tests**: All CI checks must pass
- **Review**: At least one approval required
- **Size**: Keep PRs focused and reasonably sized
- **Conflicts**: Must be resolved before merging

## Coding Standards

### C++ Style

- Use consistent indentation (2 or 4 spaces, no tabs)
- Use meaningful variable and function names
- Add comments for complex logic
- Follow existing code style in the project
- Use `const` and `constexpr` where appropriate
- Prefer RAII and smart pointers when applicable

### File Organization

```cpp
// Header file example
#ifndef FEATURE_H
#define FEATURE_H

#include <Arduino.h>
// Other includes

// Constants
#define CONSTANT_NAME 123

// Function declarations
void functionName();

#endif // FEATURE_H
```

```cpp
// Implementation file
#include "feature.h"

// Implementation
void functionName() {
    // Code here
}
```

### Configuration

- Never hardcode configuration
- Use `config.h` for user configuration
- Provide sensible defaults
- Document all configuration options

### Error Handling

- Check return values
- Log errors appropriately
- Fail gracefully when possible
- Don't crash the device

### Memory Management

- Be conscious of ESP32 memory constraints
- Free allocated memory
- Avoid memory leaks
- Use stack when possible, heap sparingly
- Be careful with string concatenation

## Testing Guidelines

### Unit Tests

Unit tests use the PlatformIO native environment:

```cpp
#include <unity.h>
#include "your_module.h"

void test_function_behavior() {
    // Arrange
    int expected = 42;
    
    // Act
    int actual = yourFunction();
    
    // Assert
    TEST_ASSERT_EQUAL(expected, actual);
}

void setUp() {
    // Setup before each test
}

void tearDown() {
    // Cleanup after each test
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function_behavior);
    UNITY_END();
}
```

### Hardware Testing

For hardware-related changes:

1. Test on actual device
2. Test both battery and USB power
3. Test sleep/wake cycles
4. Test OTA updates
5. Monitor serial output for errors
6. Check memory usage
7. Verify display updates correctly

### Test Checklist

- [ ] Unit tests added for new features
- [ ] Unit tests pass locally
- [ ] Tested on Inkplate 10 (if applicable)
- [ ] Tested on Inkplate 10v2 (if applicable)
- [ ] Tested with battery power
- [ ] Tested sleep modes
- [ ] No memory leaks
- [ ] No crashes or reboots
- [ ] Serial output clean (no errors)

## CI/CD

The project uses GitHub Actions for CI/CD. See [CI_CD_GUIDE.md](CI_CD_GUIDE.md) for detailed information.

### Workflows

- **Build and Test**: Runs on every push and PR
- **PR Validation**: Additional checks for pull requests
- **Release**: Creates releases when tags are pushed
- **Dependency Check**: Weekly dependency update checks
- **CodeQL**: Security analysis
- **Stale**: Manages stale issues and PRs

### CI Requirements

All PRs must pass:
- Unit tests
- Build for all environments
- Code quality checks
- Firmware size check
- Security scans

### Viewing CI Results

1. Go to your PR on GitHub
2. Scroll to the bottom for check status
3. Click "Details" to view logs
4. Fix any failures and push updates

## Development Tips

### Debugging

**Serial Debugging**:
```bash
# Monitor serial output
pio device monitor

# With custom baud rate
pio device monitor -b 115200
```

**Debug Build**:
```bash
# Build with debug symbols
pio run -e debug
```

**OTA Updates**:
```bash
# Update via OTA (device must be on network)
pio run -e ota -t upload
```

### Common Issues

**Build Fails - Library Not Found**:
```bash
pio pkg install
pio run --target clean
```

**Device Not Found**:
```bash
# List devices
pio device list

# Check permissions (Linux)
sudo usermod -a -G dialout $USER
```

**Out of Memory**:
- Use `String` carefully (causes fragmentation)
- Prefer `const char*` when possible
- Check heap usage regularly
- Use PSRAM for large allocations

**Touchpad Issues**:
- Calibrate on battery power
- Avoid USB interference
- Can be disabled in config

## Getting Help

- **Documentation**: Check README.md and other docs
- **Issues**: Search existing issues
- **Discussions**: Use GitHub Discussions for questions
- **Serial Output**: Always check serial logs for errors

## Recognition

Contributors will be recognized in:
- GitHub contributors list
- Release notes (for significant contributions)
- Project documentation (for major features)

## License

By contributing to HomePlate, you agree that your contributions will be licensed under the same license as the project (check LICENSE file).

## Questions?

If you have questions about contributing:

1. Check this document
2. Check existing issues and discussions
3. Create a new discussion
4. Reach out to maintainers

Thank you for contributing to HomePlate! 🎉

