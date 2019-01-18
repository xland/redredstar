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
        files: ['/project/cnblogs/xxm/nwjs/**'],
        platforms: ['osx64'],
        flavor:'sdk',
        winIco:'/project/cnblogs/xxm/icon/icon.png',
        macIcns:'/project/cnblogs/xxm/icon/icon.icns',
        cacheDir:'/project/cnblogs/xxm/cache/',
        version:'0.35.5',
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