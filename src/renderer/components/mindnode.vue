<template>
    <g :id="node.data.id" :class="nodeClass" :transform="`translate(${xValue},${yValue})`">
        <mindnode :key="item.data.id" :prop-data="item" v-for="item in node.children">
        </mindnode>
        <g @click="nodeSelect">
            <rect width="100" height="30"></rect>
            <text transform="translate(24,20)">{{node.data.text||'[未命名]'}}</text>
            <path v-if="grade > 1" :d='pathValue' style="fill: none;stroke: #0084ff"></path>
        </g>
    </g>
</template>
<script>
    import mindnode from "./mindnode"
    export default {
        name: "mindnode",
        components: {
            mindnode
        },
        props: ['propData'],
        data() {
            return {
                isSelected: false,
                grade: -1,
                node: this.propData,
                nodeClass: ['node']
            }
        },
        mounted() {
            this.cancelSelect();
            this.addSubNode();
            this.setClass();
        },
        computed: {
            xValue() {
                let x = 200;
                if (this.$parent.node.data.id == "node_0") {
                    let index = this.$parent.node.children.findIndex(v => this.node.data.id == v.data.id);
                    if (index % 2 == 0) return x;
                    else return 0 - x;
                } else {
                    if (this.$parent.xValue > 0) return x;
                    else return 0 - x;
                }
            },
            yValue() {
                let oneHeight = 30 + 60;
                let index = this.$parent.node.children.findIndex(v => this.node.data.id == v.data.id);
                let count = this.$parent.node.children.length;
                if (this.$parent.node.data.id == "node_0") {
                    if (index % 2 == 0) count = Math.ceil(count / 2);
                    else count = Math.floor(count / 2);
                    index = Math.floor(index / 2);
                }
                let totalHeight = (count - 1) * oneHeight;
                let curHeight = index * oneHeight;
                let y = curHeight - totalHeight / 2;
                return y;
            },

            pathValue() {
                let endX = 0 - this.xValue + 50;
                let endY = 0 - this.yValue + 15;
                let startX = this.xValue > 0 ? 0 : 100;
                let startY = 15;
                return `M${startX} ${startY}Q${endX} 15,${endX} ${endY}`;
            }
        },
        methods: {
            setClass() {
                this.grade = this.node.data.id.split('_').length - 1;
                if (this.grade <= 2) {
                    this.nodeClass.push('nodeGrade' + this.grade);
                } else {
                    this.nodeClass.push('nodeGradeElese');
                }
            },
            cancelSelect() {
                let self = this;
                this.bus.$on('cancelSelect', id => {
                    if (id != self.node.data.id && self.isSelected) {
                        self.isSelected = false;
                        self.nodeClass.pop();
                    }
                });
            },
            addSubNode() {
                let self = this;
                this.bus.$on('addSubNode', () => {
                    if (!self.isSelected) return;
                    let newNode = {
                        data: {
                            "id": self.node.data.id + "_" + Math.floor(Math.random() * 1000000),
                            "created": new Date().getTime(),
                            "text": "",
                            "x": 0,
                            "y": 0
                        },
                        children: []
                    }
                    self.node.children.push(newNode);
                });
            },
            nodeSelect() {
                this.bus.$emit('cancelSelect', this.node.data.id);
                if (this.isSelected) {
                    this.nodeClass.pop();
                    this.isSelected = false;
                } else {
                    this.nodeClass.push("nodeSelected");
                    this.isSelected = true;
                }
            }
        }
    }
</script>