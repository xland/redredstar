import Vue from 'vue'

import App from './App'
import router from './utils/router'
import store from './utils/store'

if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  data:store,
  components: { App },
  router,
  template: '<App/>'
}).$mount('#app')
