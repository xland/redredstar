<template>
    <g :id="node.data.id" :class="classValue" :transform="`translate(${node.data.x},${node.data.y})`">
        <node :key="item.data.id" :prop-data="item" v-for="item in node.children">
        </node>
        <g>
            <rect @click="nodeSelect" width="100" height="30"></rect>
            <text @click="nodeSelect" transform="translate(24,20)">{{node.data.text||'[未命名]'}}</text>
            <path :class="$parent.node.data.id" :d='pathValue' style="fill: none;stroke: #0084ff"></path>
            <use @click="plus" v-if="node.children_temp" y="10" :x="node.data.x > 0?-9:100" xlink:href="#plus"></use>
            <use @click="subtract" v-if="node.children.length > 1" y="10" :x="node.data.x > 0?-9:100"
                xlink:href="#subtract"></use>
        </g>
    </g>
</template>
<script>
    import node from "./node"
    import common from "./common"
    export default {
        mixins: [common],
        name: "node",
        components: {
            node
        },
        props: ['propData'],
        data() {
            return {
                isSelected: false,
                node: this.propData,
            }
        },
        mounted() {
            let self = this;
            this.bus.$on('addSubNode', () => {
                if (!self.isSelected) return;
                self.addSubNode(self.node.data.x > 0 ? 200 : -200);
            });
        },
        computed: {
            pathValue() {
                let startX = this.node.data.x > 0 ? 0 : 100;
                let startY = 15;
                let endX = 0 - this.node.data.x + 50;
                let endY = 0 - this.node.data.y + 15;
                return `M${startX} ${startY}Q${endX} 15,${endX} ${endY}`;
            },
            classValue() {
                let arr = new Set(['node']);
                this.node.data.id.split('_').length == 3 ? arr.add('nodeGrade2') : arr.delete('nodeGradeElese');
                this.isSelected ? arr.add('nodeSelected') : arr.delete('nodeSelected');
                return [...arr];
            },
        },
        methods: {
            subtract() {
                this.node.children_temp = this.node.children;
                this.node.children = [];
                this.$nextTick(() => {
                    this.$parent.reLocation(this.node.data.x > 0);
                });
            },
            plus() {
                this.node.children = this.node.children_temp;
                delete this.node.children_temp;
                this.$nextTick(() => {
                    this.reLocation(this.node.data.x > 0);
                });

            }
        }
    }
</script>