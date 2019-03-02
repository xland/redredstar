<template>
    <div id="tabbar" class="tabbar" v-if="false">
        <div @click="tabClick(index)" :class="[index==$root.u.tabIndex?'selected':'','tabItem']" v-for="(tab,index) in $root.u.tabs">
            <div :title="tab.text" class="text">{{tab.text?tab.text:'[未命名]'}}</div>
            <div @click.stop="closeTab(index)" class="tabCloseBtn" v-if="index != 0">
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

            }
        },
        methods: {
            scrollToSelectedItem() {
                this.$nextTick(function () {
                    var tb = document.getElementById("tabbar").getElementsByClassName("selected")[0];
                    tb.scrollIntoViewIfNeeded();
                })
            },
            closeTab(index) {
                this.$root.u.tabs.splice(index, 1);
                this.$root.u.tabIndex = 0;
                this.$router.push('/');
                this.scrollToSelectedItem();
            },
            tabClick(index) {
                this.$root.u.tabIndex = index;
                this.$router.push(this.$root.u.tabs[index].url);
            },
            findAndSelectTab(tab) {
                var index = this.$root.u.tabs.findIndex(v => {
                    return v.url == tab.url
                })
                if (index > 0) {
                    this.$root.u.tabIndex = index;
                    this.$router.push(tab.url);
                    this.scrollToSelectedItem();
                    return true;
                }
                return false;
            },

            addAndSelectTab(tab) {
                this.$root.u.tabs.push(tab);
                this.$root.u.tabIndex = this.$root.u.tabs.length - 1;
                this.$router.push(tab.url);
                this.scrollToSelectedItem();
            },
            removeTab(tab) {
                var index = this.$root.u.tabs.findIndex(v => {
                    return v.url == tab.url
                });
                if (index < 1) {
                    return;
                }
                this.$root.u.tabs.splice(index, 1);
                this.$root.u.tabIndex = 0;
                this.$router.push('/');
                this.scrollToSelectedItem();
            },
            hookXScroll() {
                document.getElementById('tabbar').addEventListener("mousewheel", function (e) {
                    var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
                    document.getElementById('tabbar').scrollLeft -= (delta * 40);
                    e.preventDefault();
                }, false);
            },
            hookEvent() {
                var self = this;
                self.bus.$on('addTab', function (tab) {
                    self.addAndSelectTab(tab);
                });
                self.bus.$on('findOrAddTab', function (tab) {
                    var flag = self.findAndSelectTab(tab);
                    if (!flag) {
                        self.addAndSelectTab(tab);
                    }
                });
                self.bus.$on('removeTab', function (tab) {
                    self.removeTab(tab);
                });
            }
        },
        mounted() {
            let db = this.$root.db;
            db("articles").where('tab_index', '>', -1).then(rows=>{

            })
            this.hookXScroll();
            this.scrollToSelectedItem();
            this.hookEvent();
            var startUrl = this.$root.u.tabs[this.$root.u.tabIndex].url;
            if (startUrl != "/") {
                this.$router.push(startUrl);
            }

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