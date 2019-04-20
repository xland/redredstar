import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router)

const router = new Router({
    routes: [{
            path: '/',
            redirect: '/articles'
        },{
            path:'/articles',
            component: () => import( /* webpackChunkName: "article" */ '../views/articles.vue'),
        },{
            path: '/article/:id',
            component: () => import( /* webpackChunkName: "article" */ '../views/article.vue'),
        },{
            path:'/minds',
            component: () => import( /* webpackChunkName: "mind" */ '../views/minds.vue'),
        },{
            path: '/mind/:id',
            component: () => import( /* webpackChunkName: "mind" */ '../views/mind.vue'),
        },{
            path: '/flowers',
            component: () => import( /* webpackChunkName: "flower" */ '../views/flowers.vue'),
        },{
            path: '/setting',
            component: () => import( /* webpackChunkName: "setting" */ '../views/setting.vue'),
        }
    ]
});
export default router;