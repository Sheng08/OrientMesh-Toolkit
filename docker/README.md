# Docker

* Build

```bash
docker build --no-cache -t orientmesh_toolkit  .
```

* Run

```bash
cd ..
docker run -it --name meshlib -v $(pwd):/app orientmesh_toolkit bash
```

## Notes
* If you want to run GUI, you need to install `x11` and `xauth` on your host machine.