import Vue from 'vue'
import dbInitializer from './utils/db';
import './utils/filter'
import './utils/version'
import './utils/util'
import store from './utils/store'
import router from './utils/router'
import App from './App'
const {
  ipcRenderer
} = require('electron');
import {
  ebtRenderer
} from 'electron-baidu-tongji'
const BAIDU_SITE_ID = '75bf2b763da29b214e586f7dfa74403c'
ebtRenderer(ipcRenderer, BAIDU_SITE_ID, router)

if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.config.productionTip = false

dbInitializer.init(db => {
  new Vue({
    data: store,
    components: {
      App
    },
    router,
    beforeCreate() {
      Vue.prototype.bus = this;
      Vue.prototype.db = db;
    },
    template: '<App/>'
  }).$mount('#app')
})