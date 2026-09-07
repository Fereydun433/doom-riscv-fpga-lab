# Upstream dependencies

Add DoomGeneric from the project root after initializing the parent repository:

```sh
git submodule add https://github.com/ozkl/doomgeneric.git third_party/doomgeneric
git -C third_party/doomgeneric checkout dcb7a8dbc7a16ce3dda29382ac9aae9d77d21284
```

Git creates the submodule metadata. Commit it together with the rest of the
starter. Do not copy a previously downloaded ZIP into the destination before
running these commands.

When someone clones the published repository:

```sh
git clone --recurse-submodules https://github.com/YOUR_USERNAME/doom-riscv-fpga-lab.git
```

For an existing clone:

```sh
git submodule update --init --recursive
```

Keep platform-specific integration code in the parent project. Any future
changes to the upstream submodule need their own published commit (for example
in an owned fork) before the parent can reliably refer to them.
