var http = require('http');
const host = {
    mainWindow: null,
    headerOption:{
        "Content-Type": "text/html",
        "Access-Control-Allow-Origin": "*"
    },
    okMsg:{
        "ok": true,
        "msg": "保存成功"
    },
    processPost(request, response,msgType) {
        var jsonString = '';
        request.on('data', data => jsonString += data);
        request.on('end', () => {
            let post = JSON.parse(jsonString)
            this.mainWindow.webContents.send(msgType, post);
            response.writeHead(200, this.headerOption);
            response.end(JSON.stringify(this.okMsg));
        });
    },
    start(mw) {
        this.mainWindow = mw;
        let server = http.createServer((request, response) => {
            if (request.url == "/newArticleFromWebApp") {
                this.processPost(request, response,'articleFromWebApp')
                return;
            }
            if (request.url == "/newFlowerFromWebApp") {
                this.processPost(request, response,'flowerFromWebApp')
                return;
            }
        });
        server.on("error", err => this.mainWindow.webContents.send('alertMsgFromMain', {
            icon: "info",
            text: "抱歉，文章收集失败，请重启应用后尝试或联系管理员"
        }));
        server.listen(9416);
    }
}
export default host;