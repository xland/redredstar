<template>
  <g :id="node.data.id" :class="classValue" :transform="`translate(${node.data.x},${node.data.y})`">
    <node :key="item.data.id" :prop-data="item" v-for="item in node.children"></node>
    <g class="gRec">
      <rect @click.stop="nodeClick" :width="node.data.w" :height="node.data.h"></rect>
      <text @click.stop="nodeClick" transform="translate(12,20)">{{node.data.text||'[未命名]'}}</text>
      <foreignObject
        @mousedown.stop
        v-if="isEdit"
        x="0"
        y="0"
        :width="node.data.w"
        :height="node.data.h"
      >
        <input v-model="editTxt" autofocus class="svgInput" type="text">
      </foreignObject>
      <path :d="pathValue"></path>
      <use
        @click="plus"
        v-if="node.children_temp"
        y="10"
        :x="node.data.x > 0?-9:node.data.w"
        xlink:href="#plus"
      ></use>
      <use
        @click="subtract"
        v-if="node.children.length > 1"
        y="10"
        :x="node.data.x > 0?-9:node.data.w"
        xlink:href="#subtract"
      ></use>
    </g>
  </g>
</template>
<script>
import node from "./node";
import common from "./common";
export default {
  mixins: [common],
  name: "node",
  components: {
    node
  },
  props: ["propData"],
  data() {
    return {
      isSelected: false,
      node: this.propData
    };
  },
  mounted() {
    let self = this;
    this.bus.$on("addSubNode", () => {
      if (!self.isSelected) return;
      let x = self.node.data.x > 0 ? (self.node.data.w+128) : (0-72-128);
      self.addSubNode(x);
    });
  },
  computed: {
    pathValue() {
      let startX = this.node.data.x > 0 ? 0 : this.node.data.w;
      let startY = 15;
      let endX = 0 - this.node.data.x + (this.node.data.x > 0 ? this.$parent.node.data.w : 0);
      let endY = 0 - this.node.data.y + 15;
      //return `M${startX},${startY}C${endX/2},${startY},${endX/2},${endY},${endX},${endY}`;
      return `M${startX},${startY}Q${endX},${startY},${endX},${endY}`;
    },
    classValue() {
      let arr = new Set(["node"]);
      this.node.data.id.split("_").length == 3
        ? arr.add("nodeGrade2")
        : arr.delete("nodeGradeElese");
      this.isSelected ? arr.add("nodeSelected") : arr.delete("nodeSelected");
      return [...arr];
    }
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
};
</script>
<style scoped>
.gRec path {
  fill: none;
  stroke: #0084ff;
}
</style>
