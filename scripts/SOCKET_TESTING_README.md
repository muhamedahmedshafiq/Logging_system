# Phase 2 Socket Testing Guide

## Files Provided

1. **telemetry_server.py** - Python server that sends real CPU/memory data
2. **simple_telemetry_server.sh** - Bash server that sends random floats
3. **main_improved.cpp** - Enhanced main.cpp with better socket testing

## Option 1: Python Server (Recommended)

### Setup:
```bash
# Make executable
chmod +x telemetry_server.py

# Run in one terminal
./telemetry_server.py
```

### Test:
```bash
# In another terminal, rebuild and run your app
cd build
cmake --build .
./TeleLog
```

### What it does:
- Reads real data from `/proc/loadavg` (CPU)
- Reads real data from `/proc/meminfo` (Memory)
- Sends 5 messages alternating between CPU and memory
- Automatically closes connection

---

## Option 2: Simple Bash Server

### Setup:
```bash
# Make executable
chmod +x simple_telemetry_server.sh

# Run in one terminal
./simple_telemetry_server.sh
```

**Note:** This requires `netcat` (nc) to be installed:
```bash
sudo apt-get install netcat  # Ubuntu/Debian
```

---

## Testing Steps

### Terminal 1 (Server):
```bash
cd ~/ITI/ITI_FADY/Projects/LoggingAndTelemery/project/phase_2
python3 telemetry_server.py
```

You should see:
```
✅ Telemetry server started on /tmp/telemetry.sock
Waiting for client connection...
```

### Terminal 2 (Client):
```bash
cd ~/ITI/ITI_FADY/Projects/LoggingAndTelemery/project/phase_2/build
./TeleLog
```

You should see:
```
=== Phase 2: RAII & Telemetry Source Test ===

📋 Testing File Telemetry Source...
✅ File source read successfully

🔌 Testing Socket Telemetry Source...
   (Make sure telemetry_server.py is running!)
✅ Socket connected successfully
📥 Received reading 1
📥 Received reading 2
📥 Received reading 3
📥 Received reading 4
📥 Received reading 5

📤 Routing all logs to sinks...
[Logs appear here]

✅ Phase 2 Test Complete!
```

---

## Expected Output

### In Terminal 1 (Server):
```
✅ Telemetry server started on /tmp/telemetry.sock
Waiting for client connection...
🔗 Client connected!
📤 Sent: CPU: 1.67 1.41 1.22 1/2989 39416
📤 Sent: MEM: MemTotal: 16384000 | MemFree: 8192000 | MemAvailable: 12288000
📤 Sent: CPU: 1.55 1.38 1.20 2/2990 39420
📤 Sent: MEM: MemTotal: 16384000 | MemFree: 8190000 | MemAvailable: 12286000
📤 Sent: CPU: 1.60 1.40 1.21 1/2989 39425
✅ Finished sending data
Connection closed
```

### In Terminal 2 (Client - Console):
```
2026-02-04 14:45:30 [INFO   ] [FileSource::CPU_Load] 1.67 1.41 1.22 1/2989 39416
2026-02-04 14:45:30 [INFO   ] [SocketSource::RemoteTelemetry] CPU: 1.67 1.41 1.22 1/2989 39416
2026-02-04 14:45:31 [INFO   ] [SocketSource::RemoteTelemetry] MEM: MemTotal: 16384000 | MemFree: 8192000
2026-02-04 14:45:32 [INFO   ] [SocketSource::RemoteTelemetry] CPU: 1.55 1.38 1.20 2/2990 39420
2026-02-04 14:45:33 [INFO   ] [SocketSource::RemoteTelemetry] MEM: MemTotal: 16384000 | MemFree: 8190000
2026-02-04 14:45:34 [INFO   ] [SocketSource::RemoteTelemetry] CPU: 1.60 1.40 1.21 1/2989 39425
```

---

## Troubleshooting

### "Address already in use"
```bash
rm -f /tmp/telemetry.sock
```

### "Connection refused"
- Make sure server is running first
- Check socket file exists: `ls -l /tmp/telemetry.sock`

### "nc: command not found"
```bash
sudo apt-get install netcat
```

---

## What This Demonstrates

✅ **RAII in action**: Resources automatically cleaned up
✅ **Move semantics**: `unique_ptr` transfers ownership
✅ **Unix Domain Sockets**: IPC communication
✅ **Real telemetry data**: CPU load and memory usage
✅ **Integration**: Sources → Logger → Sinks pipeline

---

## Cleanup

Stop the server with `Ctrl+C`. The socket file will be automatically removed.
