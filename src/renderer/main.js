import Vue from 'vue'
import dbInitializer from './utils/db';
import './utils/filter'
import './utils/util'
import './utils/menu'
import store from './utils/store'
import router from './utils/router'
import app from './app'
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
      app
    },
    router,
    beforeCreate() {
      Vue.prototype.bus = this;
      Vue.prototype.db = db;
    },
    template: '<app/>'
  }).$mount('#app')
})