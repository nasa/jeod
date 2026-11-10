# Security Policy

## Reporting a Vulnerability

### Security Vulnerabilities

**For security-related vulnerabilities, please report them through NASA's official Vulnerability Disclosure Program (VDP):**

Submit your report via the Bugcrowd portal: https://bugcrowd.com/engagements/nasa-vdp

The VDP is the preferred channel for all security vulnerability disclosures. This ensures your report is handled appropriately by NASA's security team.

**When reporting a security vulnerability, please include:**

- **Full steps to reproduce the vulnerability** in JEOD
- The specific JEOD version or commit hash you tested against
- Any configuration files or modifications made to the default setup
- Expected vs. actual behavior
- Potential security impact

**Important:** Vulnerabilities must be reproducible in JEOD as it is intended to be deployed. Reports based on individual applications tested in isolation with custom or modified environments that do not reflect actual JEOD behavior may not be accepted.

### General Bugs and Issues

For non-security bugs and general issues, please [open a JEOD issue](https://github.com/nasa/jeod/issues/new). Please provide as much information as possible.

## Testing

**Disclaimer: nasa/jeod is not responsible for any liability incurred as stated under the [NASA Open Source Agreement Version 1.3](https://github.com/nasa/jeod/blob/main/LICENSE).**

Testing is an important aspect our team values to improve JEOD. Several tools are used for testing:

### FlawFinder

FlawFinder is ran for every push and merge request on all branches of JEOD in the developer GitLab.

### Cppcheck

Cppcheck is ran for every push and merge request on all branches of JEOD in the developer GitLab.

For more information about Cppcheck, visit http://cppcheck.sourceforge.net/.

### Regression Testing

Regression data for each [release](https://github.com/nasa/jeod/releases) are available to the public in jeod_regression.zip

Unit tests and verification sims are ran for every push and merge request on all branches of JEOD. Results are compared to the approved regression data for each tested operating system. Differences trigger reviews to approve the change.

## Additional Support

For additional support, submit a GitHub issue.

If you wish to report a cybersecurity incident or concern, please contact the NASA Security Operations Center either by phone at 1-877-627-2732 or via email address soc@nasa.gov.