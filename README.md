# Novadesk Addon SDK

Official SDK for developing external addons for Novadesk.

## Overview

The Novadesk Addon SDK provides a complete framework and tools for creating powerful addons that seamlessly integrate with the Novadesk desktop customization platform.

## Getting Started

### Prerequisites

- Visual Studio 2019 or later
- C++ development tools
- .NET Framework (for addon configuration tools)

### Installation

1. Clone the SDK repository:
```bash
git clone https://github.com/novadesk/novadesk-addon-sdk.git
cd novadesk-addon-sdk
```

2. Build the SDK:
```powershell
.\Build.ps1
```

3. Review the example addons in the `examples/` directory

## Project Structure

- **NovadeskAPI/** - Core API definitions and interfaces
- **Addons/** - Built-in addon implementations
- **examples/** - Sample addon projects
- **Test/** - Unit tests for SDK components
- **NovadeskAddon.props** - Common build properties
- **NovadeskAddons.sln** - Visual Studio solution file

## Creating Your First Addon

1. Copy one of the example addons as a template
2. Modify the addon class to implement your custom functionality
3. Register your addon with Novadesk
4. Build and test your addon
5. Package and distribute

See the [examples/](./examples/) directory for detailed walkthroughs.

## API Documentation

The SDK exposes several key interfaces that can be found in the Novadesk Developers Docs: [Novadesk Developers Docs](https://novadesk-docs.pages.dev/developers/api/addon-api.html)

## Building Addons

Use the Visual Studio solution or PowerShell build scripts:

```powershell
# Build the entire solution
.\Build.ps1

# Build with Release configuration
.\Build.ps1 -Configuration Release
```

## Contributing

We welcome contributions to the Addon SDK!

## Support & Resources

- 📖 [Official Documentation](https://novadesk-docs.pages.dev/developers/api/addon-api.html)
- 🐛 [Issue Tracker](https://github.com/Official-Novadesk/novadesk-addon-sdk/issues)

## License

See [LICENSE](LICENSE.txt) for details.
