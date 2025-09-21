# ADDRAM

A tool to add the BonusRAM to your CIDER-equipped system's memory pool.

## What is ADDRAM?

ADDRAM is a tool that activates and adds the BonusRAM provided by your CIDER expansion board to your Amiga's memory system. BonusRAM uses the address space from $A00000 to $BEFFFF.

When you run ADDRAM, it:
1. Tests and detects available BonusRAM
2. Enables the BonusRAM hardware
3. Adds the memory to your system's Fast RAM pool
4. Optionally merges it with existing Fast RAM for better efficiency

## Before You Start

### Getting Started
- ADDRAM only adds memory that's physically present on your CIDER board
- The tool safely tests memory before adding it to avoid system conflicts
- **Safe operation**: If BonusRAM is already active, ADDRAM will detect this and exit safely

### What You'll Need
- Your CIDER-equipped Amiga
- The addram program (available in [CIDER-Software.adf](https://github.com/LIV2/CIDER-Software/releases/latest))
- A CLI/terminal or Workbench to run commands

### Prerequisites
- CIDER expansion board must be properly installed
- System should be stable with adequate power
- No conflicting memory expansions in the BonusRAM address space

## Basic Usage

### Simple Memory Addition

To add BonusRAM to your system with default settings:

```
addram
```

This will:
- Test and detect available BonusRAM
- Add it to your system as Fast RAM
- Use default priority settings

### Check What Would Happen (Dry Run)

To see what ADDRAM would do without actually changing anything:

```
addram -d
```

This is useful for:
- Checking how much BonusRAM is available
- Verifying the tool detects your CIDER board
- Testing before making actual changes

### Verbose Output

To see detailed information about what's happening:

```
addram -v
```

This shows:
- Memory testing progress
- Detected memory size
- Board information
- Priority changes

### Combined Options

```
addram -d -v
```

Shows detailed information while doing a dry run.

## Advanced Options

### Setting Memory Priority

You can control the priority of Fast RAM and BonusRAM:

```
addram -p 10
```

This sets the Fast/BonusRAM priority to 10. Higher numbers mean higher priority for memory allocation.

**Priority explained:**
- Higher priority memory gets used first by the system
- Useful for controlling which memory gets allocated for different purposes
- Default system behavior works well for most users

### Merging with Fast RAM

For optimal memory layout, you can merge BonusRAM with existing Fast RAM:

```
addram -m
```

**Benefits of merging:**
- Creates one large contiguous Fast RAM block
- More efficient memory management
- Better for applications that need large memory allocations

**Note:** This is an advanced feature that modifies memory structures more extensively.

### Combined Advanced Usage

```
addram -v -m -p 15
```

This will:
- Show verbose output
- Merge BonusRAM with Fast RAM
- Set priority to 15

## Understanding BonusRAM

### Memory Layout
- **Address Range**: $A00000 to $BEFFFF (up to ~2MB)
- **Type**: Fast RAM (32-bit, no wait states)
- **Compatibility**: Works with all Amiga software that supports Fast RAM

### How It Works
1. CIDER provides RAM in the $A00000-$BEFFFF address space
2. For RAM-equipped addresses, it provides fast memory access
3. This provides additional memory in previously unused address space

## Troubleshooting

### "Memory block already added"
- BonusRAM is already active in your system
- This is normal if you've already run ADDRAM
- **Solution**: BonusRAM is working - no action needed

### "Board not found"
- CIDER expansion board isn't detected
- **Check**: Board is properly seated and connected
- **Check**: Board firmware is up to date

### "No memory found"
- CIDER board detected but no BonusRAM available
- **Possible causes**:
  - Hardware fault with RAM

### "Detected more ram than should be in Bonus region"
- Unusual memory configuration detected
- **Solution**: Check your memory configuration and board installation

## Example Workflows

### First Time Setup
```
# 1. Check what's available (safe)
addram -d -v

# 2. Add the memory
addram -v

# 3. Verify in system (check with avail command)
avail
```

### Optimal Configuration
```
# Add BonusRAM merged with Fast RAM for best performance
addram -v -m
```

### Custom Priority Setup
```
# Set specific priority for memory management
addram -v -p 20
```

### Startup-Sequence Integration

To automatically add BonusRAM every time you boot, add this to your startup-sequence:

```
; Add BonusRAM if available
C:addram >NIL:
```

Or for verbose output during boot:
```
; Add BonusRAM with status messages
C:addram -v
```

For optimal performance with automatic merging:
```
; Add BonusRAM merged with Fast RAM
C:addram -m >NIL:
```

**Startup-sequence tips:**
- Place the addram command early in your startup-sequence
- Use `>NIL:` to suppress output during normal boot
- Remove `>NIL:` if you want to see status messages during boot

## Getting Help

If addram won't run or you get errors, try running it with help option:

```
addram -h
```

This shows all available options and their syntax.

## Technical Details

### Memory Integration
- BonusRAM is added as MEMF_FAST type memory
- Can be merged with existing Fast RAM blocks for efficiency
- Priority system ensures proper memory allocation order

## Command-line Options
```
Usage: addram [-d] [-v] [-p <Priority>] [-m] [-h]
       -d - Dry run (test without making changes)
       -v - Verbose output
       -p - Set Fast/BonusRAM priority
       -m - Merge BonusRAM with Fast RAM blocks
       -h - Show help
```

## Final Tips

1. **Start with dry run**: Use `-d -v` first to see what will happen
2. **Merge for performance**: Use `-m` for optimal memory layout
3. **Default works well**: Basic `addram` is fine for most users
4. **Check with avail**: Use the `avail` command to verify memory was added
5. **Run once**: You typically only need to run ADDRAM once per boot

## License
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


addram is licensed under the GPL-2.0 only license