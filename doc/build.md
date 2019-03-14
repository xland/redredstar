## For mac

- npm install
- npm run buildsharp
- npm install sqlite3 --build-from-source --runtime=electron --target=3.1.6 --dist-url=https://atom.io/download/electron
- npm run dev

## For win：

- npm install
- npm install --global --production windows-build-tools   （我不确定这句有没有用）
- 安装vs2015（2017不行）
- 安装python 2.7.X（注意设置path）
- npm run buildsharp
- npm install sqlite3 --build-from-source --runtime=electron --target=3.1.6 --dist-url=https://atom.io/download/electron
- npm run dev
- 如果编译不成功，你可能要尝试删掉package-lock.json；尝试npm cache clean -f;

## 求助

- 我在ubuntu下编译一直编译不成功，如果有网友愿意帮忙，请不吝赐教，我QQ：412588801