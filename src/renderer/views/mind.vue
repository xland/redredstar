<template>
    <div id="mind" class="view" v-if="mind">
        <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1"
            xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svgjs="http://svgjs.com/svgjs">
            <defs></defs>
            <mindnode :node="mind.root">
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
                mind: null,
                selectedNode: null,
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
                    if (event.keyCode == 9) {
                        let newNode = {
                            data: {
                                "id": self.selectedNode.data.id + "_" + Math.floor(Math.random() * 1000000),
                                "created": new Date().getTime(),
                                "text": "",
                                "x": 0,
                                "y": 0,
                                "isSelected": true
                            },
                            children:[]
                        }
                        self.selectedNode.data.isSelected = false;
                        self.selectedNode.children.push(newNode);
                        self.selectedNode = newNode;
                    }
                }
            },
            centerRootNode() {
                let self = this;
                this.$nextTick(() => {
                    var dom = document.getElementById("mind");
                    let y = dom.clientHeight / 2 - 16;
                    let x = dom.clientWidth / 2 - 50;
                    self.mind.root.data.x = x;
                    self.mind.root.data.y = y;
                })
            },
            getData(id) {
                this.mindPath = path.join(remote.app.getPath('userData'), "/xxm/m_" + id);
                this.mind = JSON.parse(fs.readFileSync(path.join(this.mindPath, "m.data"), this.$root.rwOption));
                this.db('minds').where("id", id).update({
                    "visited_at": new Date()
                }).then(() => {
                    this.db("minds").orderBy("visited_at", "desc").limit(8).offset(1).then(
                        recentRows => {
                            this.recentMinds = recentRows;
                        })
                });
            },
            clearSelect() {
                let recursive = function (node) {

                }
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