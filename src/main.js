import Vue from 'vue'
import './utils/util'
import store from './utils/store.js'
import router from './utils/router.js'
import "./assets/base.scss"
import App from './App'


new Vue({
  data:store,
  router,
  render: h => h(App),
  beforeCreate() {
    Vue.prototype.bus = this
  }
}).$mount('#app')