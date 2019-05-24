<template>
    <div tabindex="1" @keydown="restore" @mousewheel="wheel" @mousedown="dragStart" @mouseup="dragEnd"
        @mousemove="draging" id="mind" :class="['view',drag.ing?'drag':'']" v-if="node">
        <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1"
            xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svgjs="http://svgjs.com/svgjs">
            <defs>
                <marker id="starter" refX="-6" refY="0" 
                    markerUnits="strokeWidth" markerWidth="5"
                    markerHeight="5" orient="auto">
                    <path fill="white" stroke="gray" d="M6,0A6,6,0,1,1,-6,0A6,6,0,1,1,6,0"></path>
                </marker>
            </defs>
            <g :id="node.data.id" :class="['node','nodeGrade1',isSelected?'nodeSelected':'']" transform-origin="center"
                :transform="`scale(${scale}) translate(${node.data.x},${node.data.y})`">
                <node @mousedown.stop :key="item.data.id" :prop-data="item" v-for="item in node.children">
                </node>
                <g @click="nodeSelect">
                    <rect width="100" height="30"></rect>
                    <text transform="translate(24,20)">{{node.data.text||'[未命名]'}}</text>
                </g>
            </g>
        </svg>
    </div>
</template>
<script>
    import SVG from "svg.js"
    const fs = require('fs');
    const path = require('path');
    import node from "../components/minds/node"
    import common from "../components/minds/common"
    const {
        remote
    } = require('electron');
    export default {
        mixins: [common],
        components: {
            node
        },
        data() {
            return {
                node: null,
                recentMinds: [],
                mindPath: null,
                drag: {
                    x: 0,
                    y: 0,
                    ing: false
                },
                scale: 1
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
            restore(e) {
                if ((e.metaKey || e.ctrlKey) && event.keyCode == 27) {
                    this.scale = 1;
                    this.centerRootNode();
                }
            },
            wheel(e) {
                if (e.metaKey || e.ctrlKey) {
                    this.scale += e.deltaY / 100;
                }
            },
            dragStart(e) {
                this.drag.x = e.x;
                this.drag.y = e.y;
                this.drag.ing = true;
            },
            dragEnd() {
                this.drag.ing = false;
            },
            draging(e) {
                if (!this.drag.ing) return;
                this.node.data.x = this.node.data.x + (e.x - this.drag.x) / this.scale
                this.node.data.y = this.node.data.y + (e.y - this.drag.y) / this.scale;
                this.drag.x = e.x;
                this.drag.y = e.y;
            },
            newNode() {
                var self = this;
                document.onkeydown = function (event) {
                    if (event.keyCode != 9) return;
                    if (self.isSelected) {
                        let x = self.node.children.length % 2 == 0 ? 200 : -200;
                        self.addSubNode(x);
                        return;
                    }
                    self.bus.$emit('addSubNode');
                }
            },
            centerRootNode() {
                this.$nextTick(() => {
                    var dom = document.getElementById("mind");
                    this.node.data.x = dom.clientWidth / 2 - 50;
                    this.node.data.y = dom.clientHeight / 2 - 15;
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
            },
            reLocation(isRight) {
                this.switchPath('none');
                let index = isRight ? 0 : 1;
                let cur = this.node.children[index];
                let preHeight = document.getElementById(cur.data.id).getBBox().height;
                let y = 0
                cur.data.y = y;
                cur = this.node.children[index += 2];
                while (cur) {
                    let curHeight = document.getElementById(cur.data.id).getBBox().height;
                    y += curHeight / 2 + 60 + preHeight / 2;
                    cur.data.y = y;
                    preHeight = curHeight;
                    cur = this.node.children[index += 2];
                }
                let center = y / 2;
                index = isRight ? 0 : 1;
                cur = this.node.children[index];
                while (cur) {
                    cur.data.y -= center;
                    cur = this.node.children[index += 2];
                }
                this.switchPath('inherit');
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

    #mind:focus {
        border: none;
        outline: none;
    }

    .drag {
        cursor: move;
    }
</style>