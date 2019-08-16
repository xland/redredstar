## For mac

- npm install
- npm install sqlite3 --build-from-source --runtime=electron --target=6.0.2 --dist-url=https://atom.io/download/electron
- npm run dev

## For win：

- npm install
- npm install --global --production windows-build-tools   （需要在管理员命令行下执行，可能会导致系统重启）
- npm install -g node-gyp
- （安装visualcppbuildtools_full.exe  https://go.microsoft.com/fwlink/?LinkId=691126）
- npm install sqlite3 --build-from-source --runtime=electron --target=6.0.2 --dist-url=https://atom.io/download/electron
- npm run dev
- 如果编译不成功，你可能要尝试删掉package-lock.json；尝试npm cache clean -f;

## For linux

- aptitude install build-essential  （安装g++环境）
- npm install sqlite3 --build-from-source --runtime=electron --target=6.0.2 --dist-url=https://atom.io/download/electron
- npm run dev