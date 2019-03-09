import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router)

const router = new Router({
    routes: [{
            path: '/',
            component: () => import( /* webpackChunkName: "base" */ '../views/index.vue'),
        },
        {
            path: '/article/:id',
            component: () => import( /* webpackChunkName: "base" */ '../views/article.vue'),
        },
        {
            path: '/setting',
            component: () => import( /* webpackChunkName: "base" */ '../views/setting.vue'),
        }
    ]
});
export default router;