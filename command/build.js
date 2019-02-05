// "serve": "vue-cli-service serve",
// "build": "vue-cli-service build"
const {
    exec
} = require('child_process');
var NwBuilder = require('nw-builder');

// var arguments = process.argv.splice(2);
// if (arguments[0] != "osx" && arguments[0] != "win") {
//     console.log("请指定目标平台：osx or win");
//     return;
// }

var execCmd = function (cmdStr, onOut) {
    var cmd = exec(cmdStr);
    if(onOut){
        cmd.stdout.on('data', onOut);
    }
    cmd.stdout.pipe(process.stdout);
}

var runVue = function () {
    var vueCmd = './node_modules/.bin/vue-cli-service build';
    execCmd(vueCmd, function (data) {
        console.log(data);
        if (/.+directory is ready to be deployed.+/.test(data)) {
            runNw();
        }
    });
}
var runNw = function () {
    console.log("build nwjs");
    var nw = new NwBuilder({
        files: ['/project/xiangxuema/nwjs/start.js',
        '/project/xiangxuema/nwjs/package.json',
        '/project/xiangxuema/nwjs/ui/**',
        '!/project/xiangxuema/nwjs/ui/js/*.map'
    ],
        platforms: ['osx64','linux64'], 
        version: '0.35.5',
        flavor: 'normal',
        macIcns: '/project/xiangxuema/icon/icon.icns',
        winIco: '/project/xiangxuema/icon/icon.ico',
        cacheDir: '/project/xiangxuema/cache/',
    });
    nw.on('log', console.log);
    nw.build().then(function () {
        exec("create-dmg 想学吗.app",{cwd: '/project/xiangxuema/build/想学吗/osx64'});
        console.log("build done");
    }).catch(function (error) {
        console.error(error);
    });
}
runVue();