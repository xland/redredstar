<template>
    <div id="mind" class="view" v-if="mind">
        <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1"
            xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svgjs="http://svgjs.com/svgjs">
            <defs></defs>
            <g>
                <!-- root -->
                <g>
                    <rect width="100" height="30" x="50%" y="50%" fill="#ff0033"></rect>
                    <text>{{mind.root.data.text}}</text>
                </g>
                <mindnode :node="item" :key="item.data.id" v-for="item in mind.root.children">
                </mindnode>
            </g>

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
                mind: {
                    "root": {
                        "data": {
                            "id": "ad8b70477907",
                            "created": 1555245453,
                            "text": "新建脑图"
                        },
                        "children": [{
                            "data": {
                                "id": "btdkn5f8wfs0",
                                "created": 1555245852843,
                                "text": "allen"
                            },
                            "children": [{
                                "data": {
                                    "id": "btdknr8k4oo0",
                                    "created": 1555245900328,
                                    "text": "allen"
                                },
                                "children": []
                            }, {
                                "data": {
                                    "id": "bthjp4b9lpc0",
                                    "created": 1555649425870,
                                    "text": "分支主题",
                                    "progress": null
                                },
                                "children": []
                            }, {
                                "data": {
                                    "id": "bthjp5dn90o0",
                                    "created": 1555649428191,
                                    "text": "分支主题"
                                },
                                "children": []
                            }]
                        }, {
                            "data": {
                                "id": "bthjp68fzg80",
                                "created": 1555649430053,
                                "text": "分支主题",
                                "note": "测试测试测试！！！"
                            },
                            "children": [{
                                "data": {
                                    "id": "bthjp934b3k0",
                                    "created": 1555649436262,
                                    "text": "分支主题"
                                },
                                "children": [{
                                    "data": {
                                        "id": "bthjpa2tuqo0",
                                        "created": 1555649438421,
                                        "text": "分支主题"
                                    },
                                    "children": [{
                                        "data": {
                                            "id": "bthjpb91g140",
                                            "created": 1555649440973,
                                            "text": "分支主题"
                                        },
                                        "children": [{
                                            "data": {
                                                "id": "bthjpclq1ao0",
                                                "created": 1555649443917,
                                                "text": "分支主题"
                                            },
                                            "children": []
                                        }, {
                                            "data": {
                                                "id": "bthjpeeyeps0",
                                                "created": 1555649447861,
                                                "text": "分支主题"
                                            },
                                            "children": []
                                        }]
                                    }]
                                }]
                            }]
                        }, {
                            "data": {
                                "id": "bthjp72mgy80",
                                "created": 1555649431878,
                                "text": "分支主题"
                            },
                            "children": []
                        }, {
                            "data": {
                                "id": "bthjp7yr3zs0",
                                "created": 1555649433821,
                                "text": "分支主题"
                            },
                            "children": []
                        }]
                    },
                    "template": "default",
                    "theme": "fresh-blue",
                    "version": "1.4.43"
                }
            };
        },
        beforeRouteUpdate(to, from, next) {
            this.$refs.articleEditor.saveContent(() => {
                //this.$refs.articleEditor.destroy();
                next();
            });
        },
        beforeRouteLeave(to, from, next) {
            this.$refs.articleEditor.saveContent(() => {
                //this.$refs.articleEditor.destroy();
                next();
            });
        },
        mounted() {
            // var draw = SVG('mind').size('100%', '100%');
            // var rect = draw.rect(100, 30).attr({ fill: '#f03' }).animate().fill('#f03').move("50%","50%");
        },
        methods: {

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