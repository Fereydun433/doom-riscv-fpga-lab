# Publish from Git Bash

This workflow uses Git on your own computer and GitHub in your browser.
GitHub does not need to be connected to ChatGPT.

## 1. Prepare the folder

Extract the starter archive to a normal project folder. On Windows, a path such
as `C:\Projects\doom-riscv-fpga-lab` is convenient. Open **Git Bash Here** inside
the folder containing `README.md` and `Dockerfile`.

## 2. Create an empty repository on GitHub

Open [New repository](https://github.com/new).

| Setting | Suggested value |
| --- | --- |
| Owner | Your personal GitHub account |
| Repository name | `doom-riscv-fpga-lab` |
| Description | `FPGA learning project using Docker and RTL simulation, working toward DOOM on a RISC-V system.` |
| Visibility | Public, so the project can be linked from your CV |
| Initialize README | Off; included in this starter |
| Add .gitignore | None; included in this starter |
| Choose a license | None in the form; GPL-2.0 text is included in this starter |

Click **Create repository** and keep its HTTPS URL available.

## 3. Initialize and add the upstream source

Run these commands from the starter's root folder. This sequence is for a new
folder that is not already a Git repository:

```sh
git init -b main
git submodule add https://github.com/ozkl/doomgeneric.git third_party/doomgeneric
git -C third_party/doomgeneric checkout dcb7a8dbc7a16ce3dda29382ac9aae9d77d21284
git add .
git status --short
git commit -m "Initialize FPGA learning project and track DoomGeneric upstream"
```

The submodule checkout may display a **detached HEAD** message. That is normal
when a dependency is fixed to a particular commit.

If `git commit` reports **Author identity unknown**, set your identity for this
repository and repeat the commit command. Replace the example values first:

```sh
git config user.name "Your Name"
git config user.email "YOUR_GITHUB_COMMIT_EMAIL"
```

Your GitHub commit email can be the privacy address shown in GitHub's email
settings. Keep an existing valid Git identity if you already configured one.

## 4. Push your first commit

Replace `YOUR_USERNAME` with your actual GitHub username, or paste the exact
HTTPS URL GitHub showed after creating the repository:

```sh
git remote add origin https://github.com/YOUR_USERNAME/doom-riscv-fpga-lab.git
git push -u origin main
```

With Git Credential Manager configured, the first push can open a browser for
GitHub sign-in. Complete it on your own computer. A GitHub account password is
not used as a Git HTTPS password. If authentication fails, follow
[GitHub's Git Credential Manager instructions](https://docs.github.com/en/get-started/git-basics/caching-your-github-credentials-in-git).

Refresh the repository page. Its root should show the project README, with
DoomGeneric under `third_party/doomgeneric` pointing to the selected upstream
revision. The **Actions** tab will show the first counter simulation workflow.

If a command fails, stop at that command and inspect the error before running
the remaining commands. Do not force-push to fix an unexpected conflict.

## 5. Continue development

Run the Docker commands in the main README, record the result, and commit each
completed milestone with a message describing the actual change.

For a fresh clone on another computer:

```sh
git clone --recurse-submodules https://github.com/YOUR_USERNAME/doom-riscv-fpga-lab.git
```

Useful repository topics: `fpga`, `risc-v`, `verilator`, `systemverilog`,
`docker`, `doom`, `hardware-simulation`.

## References

- [Create a GitHub repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-new-repository)
- [Git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
