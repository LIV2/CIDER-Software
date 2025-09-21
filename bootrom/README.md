# CIDER BootROM

A resident module that automatically adds BonusRAM to your CIDER-equipped system at boot time.

## What is BootROM?

BootROM is a resident module (library) that automatically detects and adds your CIDER board's BonusRAM to the system memory pool during the boot process. Unlike the standalone `addram` tool, BootROM runs automatically every time you boot your system without requiring any user intervention.

### Key Features
- **Automatic operation**: Runs during system startup without user intervention
- **Smart detection**: Only activates if CIDER board and BonusRAM are present
- **Memory merging**: Attempts to merge BonusRAM with existing Fast RAM for optimal layout
- **Safe fallback**: Creates separate memory block if merging isn't possible
- **Boot bypass**: Hold left mouse button during boot to skip activation
- **Legacy support**: Fixes memory priorities on Kickstart 1.x systems

## How It Works

BootROM integrates into the Amiga's boot process as a resident module:

1. **Detection**: Scans for CIDER board during system startup
2. **Testing**: Safely tests available BonusRAM using non-destructive methods
3. **Integration**: Adds memory to system pool, preferring to merge with existing Fast RAM
4. **Optimization**: On older Kickstart versions, adjusts memory priorities for better performance

### Memory Layout
- **Address Range**: $A00000 to $BEFFFF (up to ~2MB)
- **Type**: Fast RAM (32-bit, no wait states)
- **Integration**: Merged with existing Fast RAM when possible
- **Compatibility**: Works with all Amiga software that supports Fast RAM

## Installation Methods

### Method 1: Extended ROM Integration (Recommended)

For permanent installation, integrate BootROM into your Extended ROM using tools like Remus or Romtool. The custom Extended ROM can then be easily programmed into your CIDER's flash memory using the cflash tool.

**Benefits:**
- Automatic activation on every boot
- No startup-sequence modification needed
- Works even with minimal system configurations
- Fastest activation (runs early in boot process)
- Easy installation to flash memory with cflash

### Method 2: LoadModule Command

For temporary or testing purposes, you can load BootROM manually:

```
loadmodule bootrom
```

**Use cases:**
- Testing before permanent installation
- Temporary setups
- Systems where you don't want to modify Extended ROM

### Method 3: Startup-Sequence Integration

Add to your startup-sequence for automatic loading:

```
; Load CIDER BootROM support
C:loadmodule DEVS:bootrom
```

## User Control

### Bypassing BootROM

To prevent BootROM from activating (useful for troubleshooting):

1. **During boot**: Hold the **left mouse button** while the system starts
2. **Keep holding** until the boot process completes
3. **Result**: BootROM will detect the button press and skip activation

This is useful when:
- Troubleshooting memory issues
- Testing system without BonusRAM
- Comparing performance with/without additional memory

### Verifying Operation

Check if BootROM successfully added memory:

```
avail
```

Look for:
- Increased Fast RAM amount
- Memory blocks labeled "GottaGoFast!"
- Total available memory increase

## Technical Details

### Resident Module Integration
- **Priority**: Runs during cold start initialization
- **Type**: NT_LIBRARY resident module
- **Activation**: Automatic during system boot
- **Dependencies**: Requires expansion.library

### Memory Testing Method
BootROM uses safe, non-destructive memory testing:
- Tests at offset +$B00 from each 64KB boundary
- Avoids potential conflicts with system hardware
- Uses address-based test patterns
- Restores original values after testing

### Memory Integration Strategy
1. **Preferred**: Merge with existing Fast RAM block for contiguous memory
2. **Fallback**: Create separate BonusRAM memory block
3. **Optimization**: Adjusts memory priorities on Kickstart 1.x systems

### Kickstart 1.x Enhancements
On older Kickstart versions, BootROM provides additional optimizations:
- Sets Ranger RAM priority to -5 for better allocation order
- Ensures proper memory type flags are set
- Improves overall memory management behavior

## Troubleshooting

### BootROM Not Running
- **Check installation**: Verify BootROM is properly integrated into Extended ROM
- **Check board**: Ensure CIDER board is properly installed and detected
- **Mouse button**: Make sure you're not accidentally holding left mouse button during boot

### Memory Not Added
- **No BonusRAM**: Board may not have RAM installed or RAM may be faulty
- **Already active**: Memory may already be added by other means (addram tool, etc.)
- **Board detection**: CIDER board may not be detected properly

### Verification
Use these commands to check system status:
```
; Check available memory
avail

; Check for CIDER board (if you have appropriate tools)
showconfig

; Check memory layout
showmem
```

## Comparison with ADDRAM Tool

| Feature | BootROM | ADDRAM Tool |
|---------|---------|-------------|
| **Activation** | Automatic | Manual |
| **Boot timing** | Early (resident) | Later (startup-sequence) |
| **User control** | Mouse button bypass | Command line options |
| **Installation** | Extended ROM | Startup-sequence |
| **Flexibility** | Fixed behavior | Configurable options |
| **Best for** | Set-and-forget | Testing and customization |

### When to Use Each

**Use BootROM when:**
- You want automatic, hands-off operation
- You have a stable system configuration
- You want the earliest possible memory activation
- You prefer ROM-based solutions

**Use ADDRAM tool when:**
- You want control over memory configuration
- You need verbose output for troubleshooting
- You want to test different settings
- You prefer startup-sequence based solutions


## Integration Examples

### Extended ROM Integration Workflow
```
# 1. Obtain bootrom module from CIDER-Software.lha
# Download from: https://github.com/LIV2/CIDER-Software/releases/latest

# 2. Backup your current Extended ROM
cp extended.rom extended.rom.backup

# 3. Add bootrom to Extended ROM using Remus/Romtool
<use your preferred ROM tool to add bootrom to extended.rom>

# 4. Flash updated Extended ROM to your CIDER
cflash -x extended.rom
```

### Testing Before Permanent Installation
```
# 1. Test with loadmodule first
loadmodule bootrom

# 2. Verify memory was added
avail

# 3. If satisfied, integrate into Extended ROM using your preferred ROM tool
```

## Final Tips

1. **Backup first**: Always backup your Extended ROM before modification
2. **Test thoroughly**: Use loadmodule to test before permanent integration
3. **Mouse bypass**: Remember the left mouse button bypass for troubleshooting
4. **Check compatibility**: Verify with your specific system configuration
5. **Monitor memory**: Use `avail` to confirm proper operation

## License
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)


BootROM is licensed under the GPL-2.0 only license