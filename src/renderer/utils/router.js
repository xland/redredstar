import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router)

const router = new Router({
    routes: [{
            path: '/',
            redirect: '/articles'
        },{
            path:'/articles',
            component: () => import( /* webpackChunkName: "base" */ '../views/articles.vue'),
        },{
            path: '/article/:id',
            component: () => import( /* webpackChunkName: "base" */ '../views/article.vue'),
        },{
            path:'/minds',
            component: () => import( /* webpackChunkName: "base" */ '../views/minds.vue'),
        },{
            path: '/mind/:id',
            component: () => import( /* webpackChunkName: "base" */ '../views/mind.vue'),
        },{
            path: '/flowers',
            component: () => import( /* webpackChunkName: "base" */ '../views/flowers.vue'),
        },{
            path: '/setting',
            component: () => import( /* webpackChunkName: "base" */ '../views/setting.vue'),
        }
    ]
});
export default router;