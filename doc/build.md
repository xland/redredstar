## For Mac And Linux

* npm install
* npm install sqlite3 --build-from-source --runtime=electron --target=2.0.17 --dist-url=https://atom.io/download/electron
* npm run dev

## For win：

* npm install
* npm install --global --production windows-build-tools   （我不确定这句有没有用）
* 安装vs2015（2017不行）
* 安装python 2.7.*（注意设置path）
* npm install sqlite3 --build-from-source --runtime=electron --target=2.0.17 --dist-url=https://atom.io/download/electron
* npm install --save-dev electron-rebuild
* electron-rebuild -f -w sharp（最好设置到package.json中，以后可能还会用到）