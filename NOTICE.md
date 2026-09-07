# Attribution and scope

The DOOM game engine was created by id Software. DoomGeneric is maintained by
ozkl and its contributors and is derived from earlier DOOM source ports.

- Upstream: https://github.com/ozkl/doomgeneric
- Initial reference: `dcb7a8dbc7a16ce3dda29382ac9aae9d77d21284`
- Intended location: `third_party/doomgeneric`, as a Git submodule
- Upstream license: see the upstream `LICENSE` and file-level notices

The starter archive does not bundle DoomGeneric source or game assets. The Git
Bash setup commands retrieve the upstream source with its history and license.
The parent repository records the selected upstream commit.

The Docker recipe, counter exercise, simulation harness, and project documents
are additions prepared for this project. They are not part of the upstream
DOOM engine. This project's integration work is still at its initial stage.

The original starter code is distributed under GPL-2.0-only. The root license
does not replace the licenses of separately maintained tools or dependencies.
No game WADs are distributed by this project.
