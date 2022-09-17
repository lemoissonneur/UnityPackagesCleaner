# UnityPackagesCleaner

UnityPackagesCleaner is a tool that allows a Unity user to remove unused packages from Unity cache.

This tool works in four steps:

* Step 1: Unity Hub logs analysis to get existing Unity projects
* Step 2: package dependencies extraction from existing Unity project `packages-lock.json` file
* Step 3: Unity cache packages listing
* Step 4: comparison to find unused packages in Unity cache

From Step 4, a user can decide to delete those unused packages by clicking delete. The tool will then show delete
packages in a 5th step.

**_NOTE:_**

* old projects may be ignored (no longer referenced in Unity Hub logs)
* projects not opened with unity hub may be ignored
* if a user decide to delete unused package, both `cache/packages` and `cache/npm` in Unity Hub cache will be cleared

## Prerequisites

In order to use this tool, ensure you have met the following requirements:

* Windows 10/11 operating system
* Existing Unity projects were opened though Unity Hub

## Compile UnityPackagesCleaner

To compile this tool you will need :
* Qt Creator >= 6.0.2
* Qt Src >= 6.2.3
* msvc2019_64

## Using UnityPackagesCleaner

To use `UnityPackagesCleaner`, simply run `UnityPackagesCleaner.exe` and follow instructions.