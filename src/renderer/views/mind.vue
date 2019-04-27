<template>
    <div id="mind" class="view" v-if="node">
        <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1"
            xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svgjs="http://svgjs.com/svgjs">
            <defs>
            </defs>
            <mindnode :prop-data="node">
            </mindnode>
        </svg>
    </div>
</template>
<script>
    import SVG from "svg.js"
    const fs = require('fs');
    const path = require('path');
    import mindnode from "../components/mindnode"
    const {
        remote
    } = require('electron');
    export default {
        components: {
            mindnode
        },
        data() {
            return {
                node: null,
                recentMinds: [],
                mindPath: null
            }
        },
        beforeRouteUpdate(to, from, next) {
            next();
            // this.$refs.articleEditor.saveContent(() => {
            //     next();
            // });
        },
        beforeRouteLeave(to, from, next) {
            next();
            // this.$refs.articleEditor.saveContent(() => {
            //     
            // });
        },
        mounted() {
            let id = this.$route.params.id;
            this.getData(id);
            this.centerRootNode();
            this.newNode();
        },
        methods: {
            newNode() {
                var self = this;
                document.onkeydown = function (event) {
                    if (event.keyCode != 9) return;
                    self.bus.$emit('addSubNode');
                }
            },
            centerRootNode() {
                this.$nextTick(() => {
                    var dom = document.getElementById("mind");
                    let y = dom.clientHeight / 2 - 16;
                    let x = dom.clientWidth / 2 - 50;
                    this.node.data.x = x;
                    this.node.data.y = y;
                })
            },
            getData(id) {
                this.mindPath = path.join(remote.app.getPath('userData'), "/xxm/m_" + id);
                let mind = fs.readFileSync(path.join(this.mindPath, "m.data"), this.$root.rwOption);
                mind = JSON.parse(mind);
                this.node = mind.root;
                this.db('minds').where("id", id).update({
                    "visited_at": new Date()
                }).then(() => {
                    this.db("minds").orderBy("visited_at", "desc").limit(8).offset(1).then(
                        recentRows => {
                            this.recentMinds = recentRows;
                        })
                });
            }
        }
    };
</script>
<style lang="scss" scoped>
    #mind {
        overflow: hidden;
        margin: 8px;
        margin-top: 0px;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
        height: calc(100% - 8px);
        border-radius: 3px;
        display: flex;
        flex-flow: column;
        background: #fff;
    }
</style>