<template>
    <g :id="node.data.id" :class="nodeClass" :transform="`translate(${leftValue},${node.data.y})`">
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
            leftValue() {
                if (this.node.data.x > 0 && this.$parent.node && this.$parent.node.data.x < 0) {
                    return 0 - this.node.data.x;
                }
                return this.node.data.x;
            },
            pathValue() {
                let distanceY = Math.abs(this.node.data.y);
                let distanceX = Math.abs(this.node.data.x);
                if (this.node.data.x > 0 && this.node.data.y > 0) {
                    let startX = 0;
                    let endX = 0 - distanceX + 50;
                    let endY = 0 - distanceY + 15;
                    return `M${startX} 15Q${endX} 15,${endX} ${endY}`;
                }
                if (this.node.data.x > 0 && this.node.data.y <= 0) {
                    let startX = 0;
                    let endX = 0 - distanceX + 50;
                    let endY = distanceY + 15;
                    return `M${startX} 15Q${endX} 15,${endX} ${endY}`;
                }
                if (this.node.data.x <= 0 && this.node.data.y <= 0) {
                    let startX = 100;
                    let endX = distanceX + 50;
                    let endY = distanceY + 15;
                    return `M${startX} 15Q${endX} 15,${endX} ${endY}`;
                }
                if (this.node.data.x <= 0 && this.node.data.y > 0) {
                    let startX = 100;
                    let endX = distanceX + 50;
                    let endY = 0 - distanceY + 15;
                    return `M${startX} 15Q${endX} 15,${endX} ${endY}`;
                }
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
            setNormalChild() {
                let count = this.node.children.length;
                let startY = 0 - ((count - 1) * 60 + count * 30) / 2 + 15;
                for (let i = 0; i < count; i++) {
                    let curNode = this.node.children[i];
                    curNode.data.x = 200;
                    curNode.data.y = startY + i * 60 + i * 30;
                }
            },
            setRootChild() {
                let count = this.node.children.length;
                let midIndex = Math.ceil(count / 2);
                let startY = 0 - ((midIndex - 1) * 60 + midIndex * 30) / 2 + 15;
                for (let i = 0; i < midIndex; i++) {
                    let curNode = this.node.children[i];
                    curNode.data.x = 200;
                    curNode.data.y = startY + i * 60 + i * 30;
                }
                let leftCount = count - midIndex;
                startY = 0 - ((leftCount - 1) * 60 + leftCount * 30) / 2 + 15;
                for (let i = 0; i < leftCount; i++) {
                    let curNode = this.node.children[midIndex + i];
                    curNode.data.x = -200;
                    curNode.data.y = startY + i * 60 + i * 30;
                }
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
                    if (self.node.data.id == "node_0") {
                        self.setRootChild()
                    } else {
                        self.setNormalChild();
                    }
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