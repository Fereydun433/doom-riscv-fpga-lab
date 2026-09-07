# Validation record

## Starter preparation

- Upstream `master` was resolved to
  `dcb7a8dbc7a16ce3dda29382ac9aae9d77d21284` using `git ls-remote`.
- The source layout and commands follow the linked Verilator, Docker, and Git
  documentation.
- The 15 starter files were checked for valid UTF-8 and consistent line endings.
  Local Markdown links and the workflow's YAML structure were checked.
  `make -n sim` successfully expanded the simulation build and run commands.
- Docker and Verilator are not installed in the preparation environment.
  The image build and RTL simulation have **not** been executed here.
- No RISC-V CPU, DOOM port, game benchmark, or FPGA deployment has been tested.

The `PASS` text in the README is the expected output of a future successful run.
The workflow becomes a source of execution evidence after its first run on
GitHub; no passing CI status is claimed in this starter.

## First run

From the project root:

```sh
docker version
docker build -t doom-fpga-lab .
docker run --rm doom-fpga-lab make doctor
docker run --rm doom-fpga-lab make sim
```

Record the result below after running these commands:

| Field | Value |
| --- | --- |
| Date | Pending |
| Project commit | Pending |
| Host OS / CPU / RAM | Pending |
| Docker version | Pending |
| Verilator version | Pending |
| Image build | Pending |
| Counter simulation | Pending |
| CI run link | Pending |

The Dockerfile fixes the Ubuntu release but uses its current package versions.
Record the reported versions when comparing later runs.
