<template>
    <div id="tabbar" class="tabbar">
        <div @click="tabClick(tab)" :class="[tab.selected?'selected':'','tabItem']" v-for="(tab,index) in tabs">
            <div :title="tab.title" class="text">{{tab.title || '[未命名]'}}</div>
            <div @click.stop="removeTab(tab)" class="tabCloseBtn" v-if="index != 0">
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
                    var tb = document.getElementById("tabbar").getElementsByClassName("selected")[0];
                    tb.scrollIntoViewIfNeeded();
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
                }).catch(e => {
                    console.error(e);
                });
                this.db('tabs').where("id", oldTab.id).update({
                    selected: false
                }).catch(e => {
                    console.error(e);
                });
                this.$router.push(tab.url);
            },
            findOrAddTab(obj) {
                var tab = this.tabs.find(v => {
                    return v.url == obj.url
                })
                var oldTab = this.tabs.find(v => v.selected);
                if (oldTab == tab) {
                    return;
                }
                oldTab.selected = false;
                if (!tab) {
                    tab = obj;
                    tab.order_num = this.tabs.length;
                    tab.selected = true
                    this.tabs.push(tab);
                    this.db('tabs').insert(tab).then(rows => {
                        tab.id = rows[0];
                    })
                }
                if (!tab.selected) {
                    tab.selected = true;
                    this.db('tabs').where("id", tab.id).update(tab).catch(e => {
                        console.error(e);
                    });
                }
                this.db('tabs').where("id", oldTab.id).update(oldTab).catch(e => {
                    console.error(e);
                });
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
                this.db("tabs").where({
                    id: obj.id
                }).del().then(() => {
                    if (!this.tabs.some(v => v.selected)) {
                        this.tabs[0].selected = true;
                        this.$router.push('/');
                    }
                    this.scrollToSelectedItem();
                    this.tabs.forEach((v, i) => {
                        this.db('tabs').where("id", v.id).update({
                            order_num:i
                        }).then();
                    });
                });
                //todo
                this.$root.batchUpdate("tabs", this.tabs);
            },
            hookXScroll() {
                document.getElementById('tabbar').addEventListener("mousewheel", function (e) {
                    var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
                    document.getElementById('tabbar').scrollLeft -= (delta * 40);
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
                    this.tabs.find(v => v.selected).title = title;
                })
            }
        },
        mounted() {
            this.db("tabs").select("*").then(rows => {
                this.tabs = rows;
                var tab = rows.find(v => v.selected);
                this.$router.push(tab.url);
                this.hookXScroll();
                this.scrollToSelectedItem();
                this.hookEvent();
            })
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
        margin-bottom: 8px;
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