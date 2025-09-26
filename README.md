# latency

Security
--------

This repo previously contained a hardcoded access token in `up.js`. Do not
commit secrets to the repository. Store tokens in environment variables or a
server-side proxy, for example:

- Set an environment variable locally: `export PARTICLE_ACCESS_TOKEN=...` (macOS/Linux)
- On Windows PowerShell: `$env:PARTICLE_ACCESS_TOKEN = '...'`

In production, use a secret manager or server-side logic to avoid exposing
credentials in client-side code.