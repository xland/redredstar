// "serve": "vue-cli-service serve",
// "build": "vue-cli-service build"
const {
    exec
} = require('child_process');
var NwBuilder = require('nw-builder');

var execCmd = function(cmdStr,onOut){
    var cmd = exec(cmdStr);
    cmd.stdout.on('data', onOut);
    cmd.stdout.pipe(process.stdout);
}

var runVue = function () {
    var vueCmd = './node_modules/.bin/vue-cli-service serve';
    execCmd(vueCmd,function (data) {
        if (/.+running at.+/.test(data)) {
            runNw();
        }
    });
}
var runNw = function(){
    console.log("starting nwjs");
    var nw = new NwBuilder({
        files: ['/project/xiangxuema/nwjs/**'],
        platforms: ['osx64'],
        flavor:'sdk',
        winIco:'/project/xiangxuema/icon/icon.png',
        macIcns:'/project/xiangxuema/icon/icon.icns',
        cacheDir:'/project/xiangxuema/cache/',
        version:'0.36.1',
    });
    nw.on('log', console.log);
    nw.run().then(function () {
        console.log('all done!');
    }).catch(function (error) {
        console.error(error);
    });
}
runVue();


//;