const session = require('electron').remote.session;


setInterval(function(){
	document.querySelector("[data-cpid='C84F80D918DEC0BD7BBEB4C5EAA15CF5_babel']").click();
},100)

setInterval(function(){
	let closeBtn = document.querySelector(".mask .layer-close");
	if(closeBtn){
		closeBtn.click();
	}
},100)

// let breakPoint = function(){
//     debugger;
//     setTimeout(breakPoint,300);
// }
// if(window.location.href.includes('/item.htm')){
//     breakPoint();
// }


// session.defaultSession.cookies.get({}, (error, cookies) => {
//     console.log(error, cookies)
//   })





// oSession.ResponseHeaders.Add("Set-Cookie","dnk=xlandxland; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");
// oSession.ResponseHeaders.Add("Set-Cookie","_nk_=xlandxland; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");
// oSession.ResponseHeaders.Add("Set-Cookie","tracknick=xlandxland; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");
// oSession.ResponseHeaders.Add("Set-Cookie","lgc=xlandxland; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");
// oSession.ResponseHeaders.Add("Set-Cookie","unb=131111500; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");
// oSession.ResponseHeaders.Add("Set-Cookie","enc=TLDab%2FqWhL0CVS7Y2EudGyiGyoyIhQp6mxjunsU6lHxADDF7TLMdpxwdsvONYinTr2VfxcHfznhJJqtHtG%2BtXg%3D%3D; expires=Sun, 27-May-29 12:36:03 GMT; path=/; domain=.taobao.com;HTTPOnly");





let hack = function () {
    if (!window.location.href.includes('/item.htm')) return;
    let domains = ['domain=.tmall.com', 'domain=.taobao.com'];
    let id = '17681613';
    let udpid = 'UoYY61yZn2I%3D';  //此值只有登录之后才能获取
    let nick = "%5Cu5218%5Cu7B71%5Cu4F26";
    // //domain = 'domain=.taobao.com'
    // domains.forEach(domain => {
        document.cookie = "dnk=" + nick + ";" + domain + ";path=/";
        document.cookie = "_nk_=" + nick + ";" + domain + ";path=/";
        document.cookie = "tracknick=" + nick + ";" + domain + ";path=/";
        document.cookie = "lgc=" + nick + ";" + domain + ";path=/";
        document.cookie = "unb=" + id + ";" + domain + ";path=/";
        document.cookie = "munb=" + id + ";" + domain + ";path=/";
        document.cookie = "existShop=MTU1ODkyNTgwMg%3D%3D;" + domain + ";path=/";
        document.cookie = "_l_g_=Ug%3D%3D;" + domain + ";path=/";
        document.cookie = "tg=0;" + domain + ";path=/";
        document.cookie = "enc=TLDab%2FqWhL0CVS7Y2EudGyiGyoyIhQp6mxjunsU6lHxADDF7TLMdpxwdsvONYinTr2VfxcHfznhJJqtHtG%2BtXg%3D%3D;" + domain + ";path=/";
        document.cookie = "skt=70ed610906312824;" + domain + ";path=/";
        document.cookie = "csg=2781c416;" + domain + ";path=/";
    //     // document.cookie = "cookie17=" + udpid + ";" + domain + ";path=/";
    //     // document.cookie = "uc3=vt3=F8dBy3vPh4zzLQm4BW8%3D&udpid=" + udpid + "&nk2=qVKSVUzY&lg2=URm48syIIVrSKA%3D%3D;" + domain + ";path=/";
    // });
    // if (window.location.href.startsWith('https://item.taobao.com/item.htm')) {

    // id = '131111500';
    // //udpid = 'UoLdHkGCl1hA';
    // udpid = "UoYY61yZn2I%3D"
    // nick = "xlandxland";
    // domain = 'domain=.taobao.com'
    // let title = encodeURI(document.title);
    // let cache = Math.floor(268435456 * Math.random()).toString(16);
    // let micData = document.querySelector("meta[name='microscope-data']").content; //pageId=1481808954;prototypeId=2;siteId=4;shopId=292530791;userid=1932877612;
    // eval(micData);
    // function o(t) { function e(t) { var e = "0123456789abcdefhijklmnopqrstuvwxyzABCDEFHIJKLMNOPQRSTUVWXYZ", n = "0123456789abcdefghijkmnopqrstuvwxyzABCDEFGHIJKMNOPQRSTUVWXYZ"; return 1 == t ? e.substr(Math.floor(60 * Math.random()), 1) : 2 == t ? n.substr(Math.floor(60 * Math.random()), 1) : "0" } for (var n, o = "", a = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", r = !1; o.length < t;)n = a.substr(Math.floor(62 * Math.random()), 1), !r && o.length <= 2 && ("g" == n.toLowerCase() || "l" == n.toLowerCase()) && (0 === o.length && "g" == n.toLowerCase() ? Math.random() < .5 && (n = e(1), r = !0) : 1 == o.length && "l" == n.toLowerCase() && "g" == o.charAt(0).toLowerCase() && (n = e(2), r = !0)), o += n; return o }
    // let spmCnt = `a220o.1000855.0.0.${o(6) + o(8)}`;
    // spmCnt = '2013.1.0.0.2365457dpHVNkn';
    // let pre = encodeURIComponent('https://www.taobao.com/?spm=a2107.1.1000340.1.2b0c11d9sKSgUD');
    // let spmUrl = 'a219r.lm874.0.0';
    // let spmPre = 'a2107.1.1000340.1.2b0c11d9sKSgUD';
    // let params = document.querySelector('#tb-beacon-aplus').getAttribute("exparams");
    // //category=item%5f50000671&userid=17681613&at_isb=0&at_autype=5%5f292530791&aplus&udpid=UoYY61yZn2I%253D&at_alis=1%5f1932877612&&yunid=&7b7be6b7af911&trid=7ae4fa1515591165622235547e&asid=Ac3MDQESO+5cJsLuJwAAAACo1U7CdTCtOw==&sidx=i7C81dR2xBNtMzvoplmLw5sPvxeVpskElokaEj+wWEMv0Z8CQQLdD/0olNrXFClcvGmkPhVRuWMXSHeY7QHTr8GUJr/xB/t5w1fo+Qig3rYhhSrSwwhB4uf724XHeZzjcWNxR1yRXS0YhVTAjrABQZoD7356abHYN1rSCG9otuJoZvmLzSSZ4fVNX7u+aIFo5atz/JKq8AsWhj05T52GYvYJ+6+9xpzNlwrpUq/7Fzdqi4sNGYcUDtnYjHa236eSwQqDAqRquZkdFf/kGt60zyCUF2I+kCGo+ceV/RuX6wiHlELrjBO8Xnh07qbETF12B3PF6p01bYhUn06wcvTngw==
    // params = params.replace(/userid=\d+?&/gi, `uidaplus=${id}&`);
    // debugger;
    // let url = `https://log.mmstat.com/v.gif?logtype=1&title=${title}&pre=${pre}&scr=1920x1080&cna=oSF1FYLsGWMCAdy45bLTvV6B&nick=${nick}&wm_pageid=${pageId}&wm_prototypeid=${prototypeId}&wm_sid=${shopId}&spm-url=${spmUrl}&spm-pre=${spmPre}&spm-cnt=${spmCnt}&${params}&p=1&o=win10&b=chrome74&s=1920x1080&w=webkit&ism=pc&cache=${cache}&lver=8.11.5&jsver=aplus_std&pver=0.7.1&thw=cn&aws=1&_hng=CN%25257Czh-CN%25257CCNY%25257C156&tag=1&stag=-1&lstag=-1&_slog=0`;
    // console.log(url);
    // debugger;
    // var img = document.createElement("img");
    // img.src = url;
    // document.body.appendChild(img);
    //https://log.mmstat.com/v.gif?logtype=1&title=%E3%80%90%E8%8E%BA%E6%AD%8C%E3%80%91%E5%8D%8E%E4%B8%BD%E6%9E%97%E5%9D%8A2019%E6%98%A5%E5%A4%8F%E6%96%B0%E6%AC%BE%E6%97%85%E8%A1%8C%E6%96%87%E8%89%BA%E4%BA%9A%E9%BA%BB%E5%8D%B0%E8%8A%B1%E6%B8%85%E6%96%B0%E5%A4%8D%E5%8F%A4%E5%B0%8F%E4%B8%8A%E8%A1%A3-%E6%B7%98%E5%AE%9D%E7%BD%91&pre=https%3A%2F%2Fwww.taobao.com%2Fmarkets%2Ffootmark%2Ftbfoot%3Fspm%3Da1z0d.6639537.1997525045.3.6dea7484lsqVmj&scr=1920x1080&cna=oSF1FYLsGWMCAdy45bLTvV6B&nick=%5Cu5218%5Cu7B71%5Cu4F26&wm_pageid=1481808954&wm_prototypeid=2&wm_sid=292530791&spm-url=a1z2k.11010449.931864.2.693f509d5lJWe0&spm-pre=a1z0d.6639537.1997525045.3.6dea7484lsqVmj&spm-cnt=2013.1.0.0.577f244bScamH3&category=item%5f50000671&uidaplus=17681613&at_isb=0&at_autype=5%5f292530791&aplus&udpid=UoYY61yZn2I%253D&at_alis=1%5f1932877612&&yunid=&7b7be6b7af911&trid=7ae4fa1515591165622235547e&asid=Ac3MDQESO+5cJsLuJwAAAACo1U7CdTCtOw==&sidx=i7C81dR2xBNtMzvoplmLw5sPvxeVpskElokaEj+wWEMv0Z8CQQLdD/0olNrXFClcvGmkPhVRuWMXSHeY7QHTr8GUJr/xB/t5w1fo+Qig3rYhhSrSwwhB4uf724XHeZzjcWNxR1yRXS0YhVTAjrABQZoD7356abHYN1rSCG9otuJoZvmLzSSZ4fVNX7u+aIFo5atz/JKq8AsWhj05T52GYvYJ+6+9xpzNlwrpUq/7Fzdqi4sNGYcUDtnYjHa236eSwQqDAqRquZkdFf/kGt60zyCUF2I+kCGo+ceV/RuX6wiHlELrjBO8Xnh07qbETF12B3PF6p01bYhUn06wcvTngw==&p=1&o=win10&b=chrome74&s=1920x1080&w=webkit&ism=pc&cache=77676f7&lver=8.11.5&jsver=aplus_std&pver=0.7.1&thw=cn&aws=1&_hng=CN%25257Czh-CN%25257CCNY%25257C156&tag=1&stag=-1&lstag=-1&_slog=0
    // }
    // domains.forEach(domain => {
    //     document.cookie = "dnk=" + nick + ";" + domain;
    //     document.cookie = "_nk_=" + nick + ";" + domain;
    //     document.cookie = "tracknick=" + nick + ";" + domain;
    //     document.cookie = "lgc=" + nick + ";" + domain;
    //     document.cookie = "unb=" + id + ";" + domain;
    //     document.cookie = "existShop=MTU1ODkyNTgwMg%3D%3D;" + domain;
    //     document.cookie = "_l_g_=Ug%3D%3D;" + domain;
    //     document.cookie = "tg=0;" + domain;

    //     document.cookie = "enc=TLDab%2FqWhL0CVS7Y2EudGyiGyoyIhQp6mxjunsU6lHxADDF7TLMdpxwdsvONYinTr2VfxcHfznhJJqtHtG%2BtXg%3D%3D;" + domain;
    //     document.cookie = "skt=70ed610906312824;" + domain;
    //     document.cookie = "csg=2781c416;" + domain;

    //     document.cookie = "cookie17=" + udpid + ";" + domain;
    //     document.cookie = "uc3=vt3=F8dBy3vPh4zzLQm4BW8%3D&udpid=" + udpid + "&nk2=qVKSVUzY&lg2=URm48syIIVrSKA%3D%3D;" + domain;

    //     // document.cookie = "sg=m23;" + domain;
    //     // document.cookie = "_cc_=VFC%2FuZ9ajQ%3D%3D;" + domain;

    //     // document.cookie = "cookie1=BdWMlr3BhLgCeREByUBXLutQhNFM8DgHv1F1boDtAow%3D;" + domain;
    //     // document.cookie = "uc1=cookie16=U%2BGCWk%2F74Mx5tgzv3dWpnhjPaQ%3D%3D&cookie21=U%2BGCWk%2F7oPZZnSPh0Og%3D&cookie15=V32FPkk%2Fw0dUvg%3D%3D&existShop=false&pas=0&cookie14=UoTZ7HyPZhh5LA%3D%3D&tag=8&lng=zh_CN;" + domain;
    //     // document.cookie = "login=true;" + domain;
    //     // document.cookie = "miid=398825781324972039;" + domain;
    //     // document.cookie = "_l_g_=Ug%3D%3D;" + domain;
    //     // document.cookie = "sg=%E4%BC%A635;" + domain;
    //     // document.cookie = "_cc_=U%2BGCWk%2F7og%3D%3D;" + domain;
    //     // document.cookie = "csg=55b28a96;" + domain;
    //     // document.cookie = "existShop=MTU1ODg3OTE0OQ%3D%3D;" + domain;
    //     // document.cookie = "tg=0;" + domain;
    //     // document.cookie = "UM_distinctid=16a47cc9de319d-07005c81e5082-3b7a516b-fa000-16a47cc9de65ca;" + domain;
    // })



    // let script = document.querySelector("body script");
    // let queryStr = script.getAttribute("exparams");
    // let uid = queryStr.split("userid=")[1].split("&")[0];
    // console.log("uid:" + uid);
    // if (uid) {
    //     console.log("in.............................................")
    // }
}

window.onload = hack;

window.onbeforeunload = function () {
    // debugger;
}