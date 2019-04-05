<template>
    <div v-show="tabs.length>1" class="tabbar">
        <div @click="tabClick(tab)" :class="[tab.selected?'selected':'','tabItem']" v-for="(tab,index) in tabs">
            <div v-if="tab.title" :title="tab.title" class="text">{{tab.title}}</div>
            <div v-else :title="tab.title" class="text">[未命名]</div>
            <div @click.stop="removeTab(tab)" class="tabCloseBtn">
                <i class="iconfont icon-guanbi tabCloseIcon"></i>
            </div>
        </div>
    </div>
</template>
<script>
    //todo:右边的边框不精致
    export default {
        data() {
            return {
                tabs: []
            }
        },
        methods: {
            scrollToSelectedItem() {
                this.$nextTick(function () {
                    var tb = document.querySelector(".tabbar").getElementsByClassName("selected")[0];
                    if (tb) {
                        tb.scrollIntoViewIfNeeded();
                    }
                })
            },
            tabClick(tab) {
                if (tab.selected) {
                    return;
                }
                var oldTab = this.tabs.find(v => v.selected);
                oldTab.selected = false;
                tab.selected = true;
                this.db('tabs').where("id", tab.id).update({
                    selected: true
                }).then();
                this.db('tabs').where("id", oldTab.id).update({
                    selected: false
                }).then();
                this.$router.push(tab.url);
            },
            findOrAddTab(obj) {
                var tab = this.tabs.find(v => {
                    return v.url == obj.url
                });
                if (!tab) {
                    tab = obj;
                    tab.order_num = this.tabs.length;
                    tab.selected = true;
                    this.tabs.push(tab);
                    this.db('tabs').insert(tab).then(rows => {
                        tab.id = rows[0];
                    })
                } else {
                    var curSelectedTab = this.tabs.find(v => v.selected);
                    if (curSelectedTab == tab) {
                        return;
                    }
                    if (curSelectedTab) {
                        curSelectedTab.selected = false;
                        this.db('tabs').where("id", curSelectedTab.id).update(curSelectedTab).then();
                    }
                }
                if (!tab.selected) {
                    tab.selected = true;
                    this.db('tabs').where("id", tab.id).update(tab).then();
                }
                this.$router.push(tab.url);
                this.scrollToSelectedItem();
            },
            removeTab(obj) {
                var index = this.tabs.findIndex(v => {
                    return v.url == obj.url
                });
                if (index < 0) {
                    return;
                }
                obj = this.tabs.splice(index, 1)[0];
                if (!this.tabs.some(v => v.selected)) {
                    this.$router.push('/');
                }
                this.scrollToSelectedItem();
                this.tabs.forEach((v, i) => {
                    this.db('tabs').where("id", v.id).update(v).then();
                });
                this.db("tabs").where({
                    id: obj.id
                }).del().then();
            },
            hookXScroll() {
                document.querySelector('.tabbar').addEventListener("mousewheel", function (e) {
                    var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
                    document.querySelector('.tabbar').scrollLeft -= (delta * 40);
                    e.preventDefault();
                }, false);
            },
            hookEvent() {
                this.bus.$on('findOrAddTab', tab => {
                    this.findOrAddTab(tab);
                });
                this.bus.$on('removeTab', tab => {
                    this.removeTab(tab);
                });
                this.bus.$on("changeTitle", title => {
                    let tab = this.tabs.find(v => v.selected);
                    tab.title = title;
                    this.db('tabs').where("id", tab.id).update(tab).then();
                })
                this.bus.$on("deselectTab", () => {
                    let tab = this.tabs.find(v => v.selected);
                    if (!tab) {
                        return;
                    }
                    tab.selected = false;
                    this.db('tabs').where("id", tab.id).update(tab).then();
                })
            },
            init() {
                this.db("tabs").select("*").then(rows => {
                    this.tabs = rows;
                    var tab = rows.find(v => v.selected);
                    if (tab) {
                        this.$router.push(tab.url);
                        this.scrollToSelectedItem();
                    } else {
                        this.$router.push("/");
                    }
                })
            }
        },
        mounted() {
            this.hookXScroll();
            this.hookEvent();
            this.init();
        }
    }
</script>
<style lang="scss" scoped>
    .tabbar::-webkit-scrollbar {
        height: 2px;
        background-color: #f6f6f6;
    }

    .tabbar::-webkit-scrollbar-track {
        background: #fff;
    }

    .tabbar::-webkit-scrollbar-thumb {
        border-radius: 2px;
        background-color: #bbb;
    }

    .tabbar {
        background: #fff;
        height: 32px;
        line-height: 32px;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.1);
        width: 100%;
        overflow-y: hidden;
        display: -webkit-box;
        overflow-x: hidden;
        border-top: 1px solid #efefef;
    }

    .tabbar:hover {
        overflow-x: auto;
    }

    .tabItem {
        color: #666;
        height: 30px;
        line-height: 32px;
        border-right: 1px solid #f6f6f6;
        border-bottom: 2px solid #fff;
        overflow: hidden;
        cursor: pointer;
        float: none;
    }

    .selected {
        background: #f6f6f6;
        border-bottom: 2px solid #1787fb;
    }

    .text {
        float: left;
        max-width: 120px;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
        display: block;
        font-size: 12px;
        color: #333;
        padding-left: 8px;
        padding-right: 6px;
    }


    .tabCloseBtn {
        padding-right: 3px;
        height: 30px;
        float: left;
        text-align: center;
        color: #bbb;
    }

    .tabCloseBtn:hover {
        color: #ee4242;
    }

    .tabCloseIcon {
        font-size: 17px !important;
    }
</style>