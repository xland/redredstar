import Vue from 'vue'
import store from './utils/store'
import router from './utils/router'
import App from './App'

if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  data:store,
  components: { App },
  router,
  beforeCreate() {
    Vue.prototype.bus = this
  },
  template: '<App/>'
}).$mount('#app')
