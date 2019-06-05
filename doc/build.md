## For mac

- npm install
- npm run buildsharp
- npm install sqlite3 --build-from-source --runtime=electron --target=5.0.2 --dist-url=https://atom.io/download/electron
- npm run dev

## For win：

- npm install
- npm install --global --production windows-build-tools   （需要在管理员命令行下执行，可能会导致系统重启）
- npm install -g node-gyp
- （安装visualcppbuildtools_full.exe  https://go.microsoft.com/fwlink/?LinkId=691126）
- npm install sqlite3 --build-from-source --runtime=electron --target=5.0.2 --dist-url=https://atom.io/download/electron
- npm run buildsharp    （需要在管理员命令行下执行）
- npm run dev
- 如果编译不成功，你可能要尝试删掉package-lock.json；尝试npm cache clean -f;

## 求助

- 我在ubuntu下编译一直编译不成功，如果有网友愿意帮忙，请不吝赐教，我QQ：412588801