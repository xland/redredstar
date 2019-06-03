<template>
  <div tabindex="1" id="mind" :class="['view',drag.ing?'drag':'']" v-if="node">
    <div class="tools">
      <div class="toolBtn" @mouseenter="showRecent = true" @mouseleave="showRecent = false">
        <i class="iconfont icon-zuijin icon"></i>
      </div>
      <div class="toolBtn" style="border-top-right-radius: 4px;" @click="showHelp">
        <i class="iconfont icon-help icon"></i>
      </div>
    </div>
    <div v-show="showRecent" class="recent" @mouseenter="showRecent = true" @mouseleave="showRecent = false" style="top:44px;">
      <div :key="item.id" @click="$router.push('/mind/' + item.id)" v-for="item in recent" class="item">{{item.title}}</div>
    </div>
    <div style="display: none">
      <div id="helpContainer">
        在画布空白处按住鼠标左键拖动画布；
        <br>选中节点后，按Tab键增加子节点；
        <br>选中节点后，按Del(Backspace)键删除节点；
        <br>双击节点，修改节点内文字；
        <br>按住Ctrl(⌘)键，滚动鼠标滚轮，放大或缩小画布；
        <br>双击画布空白处，还原画布状态；
        <br>
      </div>
    </div>
    <svg
      width="100%"
      height="100%"
      xmlns="http://www.w3.org/2000/svg"
      version="1.1"
      xmlns:xlink="http://www.w3.org/1999/xlink"
      xmlns:svgjs="http://svgjs.com/svgjs"
      @mousewheel="wheel"
      @mousedown.self="dragStart"
      @mouseup="dragEnd"
      @mousemove="draging"
    >
      <defs>
        <symbol id="plus">
          <circle cx="5" cy="5" r="4" fill="#00000000" stroke="#0084ff"></circle>
          <path d="M5,1L5,9" stroke="#0084ff"></path>
        </symbol>
        <symbol id="subtract">
          <circle cx="5" cy="5" r="4" fill="#00000000" stroke="#0084ff"></circle>
        </symbol>
      </defs>
      <g
        :id="node.data.id"
        :class="['node','nodeGrade1',isSelected?'nodeSelected':'']"
        transform-origin="center"
        :transform="`scale(${scale}) translate(${node.data.x},${node.data.y})`"
      >
        <g class="gChild">
          <node @mousedown.stop :key="item.data.id" :prop-data="item" v-for="item in node.children"></node>
        </g>
        <g class="gRec">
          <rect @click.stop="nodeClick" :width="node.data.w" :height="node.data.h"></rect>
          <text @click.stop="nodeClick" transform="translate(12,20)">{{node.data.text||'[未命名]'}}</text>
          <foreignObject
            @mousedown.stop
            @keydown.stop
            v-show="isEdit"
            x="0"
            y="0"
            :width="node.data.w"
            :height="node.data.h"
          >
            <input @change="titleChange" v-model="editTxt" class="svgInput" type="text"></input>
          </foreignObject>
        </g>
      </g>
    </svg>
  </div>
</template>
<script>
import SVG from "svg.js";
const fs = require("fs");
const path = require("path");
import node from "../components/minds/node";
import common from "../components/minds/common";
import { truncate } from "fs";
const { remote } = require("electron");
export default {
  mixins: [common],
  components: {
    node
  },
  data() {
    return {
      node: null,
      mindPath: null,
      recent:[],
      showRecent:false,
      drag: {
        x: 0,
        y: 0,
        ing: false
      },
      scale: 1,
      tick: null,
      needSave: false,
    };
  },
  beforeRouteUpdate(to, from, next) {
    this.save();
    clearInterval(this.tick);
    next();
  },
  beforeRouteLeave(to, from, next) {
    this.save();
    clearInterval(this.tick);
    next();
  },
  mounted() {
    let id = this.$route.params.id;
    this.getData(id);
    this.centerRootNode();
    this.hookKeyDown();
    this.tick = setInterval(() => {
      this.save();
    }, this.$root.tickStep);
    this.bus.$on("needSave", () => {
      this.needSave = true;
    });
  },
  methods: {
    save(cb) {
      if (!this.needSave) return;
      this.bus.$emit('saving');
      let obj = {
        template: "default",
        theme: "default",
        version: "1.4.43",
        root: this.node
      };
      fs.writeFileSync(
        path.join(this.mindPath, "m.data"),
        JSON.stringify(obj),
        this.$root.rwOption
      );
      this.needSave = false;
    },
    titleChange() {
      let id = this.$route.params.id;
      this.db("minds")
        .where("id", id)
        .update({
          title: this.editTxt
        })
        .then();
    },
    showHelp() {
      swal({
        width: 580,
        content: document.getElementById("helpContainer"),
        buttons: false
      }).then(() => {
        this.qrCodeId = null;
      });
    },
    wheel(e) {
      if (e.metaKey || e.ctrlKey) {
        this.scale += 0 - e.deltaY / 1000;
      } else {
        this.node.data.y += 0 - e.deltaY;
      }
    },
    dragStart(e) {
      let ts = new Date().getTime();
      let span = ts - this.timeStamp;
      this.timeStamp = ts;
      if (span < 300) {
        this.scale = 1;
        this.centerRootNode();
        return;
      }
      this.bus.$emit("cancelSelect", -1);
      this.drag.x = e.x;
      this.drag.y = e.y;
      this.drag.ing = true;
    },
    dragEnd() {
      this.drag.ing = false;
      this.bus.$emit("needSave");
    },
    draging(e) {
      if (!this.drag.ing) return;
      this.node.data.x = this.node.data.x + (e.x - this.drag.x) / this.scale;
      this.node.data.y = this.node.data.y + (e.y - this.drag.y) / this.scale;
      this.drag.x = e.x;
      this.drag.y = e.y;
    },
    hookKeyDown() {
      var self = this;
      document.onkeydown = function(event) {
        if (event.keyCode == 9) {
          self.bus.$emit("addSubNode");
          if (!self.isSelected) return;
          let rightSideNodes = self.node.children.filter(v => v.data.x > 0);
          let newNodeAtRight =
            rightSideNodes.length <= self.node.children.length / 2;
          let x = newNodeAtRight ? self.node.data.w + 128 : 0 - 72 - 128;
          self.addSubNode(x);
        }
        if (event.keyCode == 46 || event.keyCode == 8) {
          self.bus.$emit("delNode");
        }
        if (event.keyCode == 83 && (event.metaKey || event.ctrlKey)) {
          self.save();
        }
      };
    },
    centerRootNode() {
      this.$nextTick(() => {
        var dom = document.getElementById("mind");
        this.node.data.x = dom.clientWidth / 2 - 50;
        this.node.data.y = dom.clientHeight / 2 - 15;
        this.bus.$emit("needSave");
      });
    },
    getData(id) {
      this.mindPath = path.join(remote.app.getPath("userData"), "/xxm/m_" + id);
      let mind = fs.readFileSync(
        path.join(this.mindPath, "m.data"),
        this.$root.rwOption
      );
      mind = JSON.parse(mind);
      this.node = mind.root;
      this.db("minds")
        .where("id", id)
        .update({
          visited_at: new Date()
        })
        .then(() => {
          this.db("minds")
            .orderBy("visited_at", "desc")
            .limit(8)
            .offset(1)
            .then(recentRows => {
              this.recent = recentRows;
            });
        });
    },
    reLocation(isRight) {
      let curSideNodes = this.node.children.filter(
        v => v.data.x > 0 == isRight
      );
      if (curSideNodes.length < 1) return;
      this.switchPath("none");
      let index = 0;
      let cur = curSideNodes[index];
      let preHeight = this.getNodeHeight(cur);
      let y = 0;
      cur.data.y = y;
      cur = curSideNodes[(index += 1)];
      while (cur) {
        let curHeight = this.getNodeHeight(cur);
        y += curHeight / 2 + 60 + preHeight / 2;
        cur.data.y = y;
        preHeight = curHeight;
        cur = curSideNodes[(index += 1)];
      }
      let center = y / 2;
      index = 0;
      cur = curSideNodes[index];
      while (cur) {
        cur.data.y -= center;
        cur = curSideNodes[(index += 1)];
      }
      this.switchPath("inherit");
      this.bus.$emit("needSave");
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
.tools {
  position: absolute;
  right: 8px;
  top: 8px;
  z-index: 99;
  color: #0084ff;
  text-align: center;
  line-height: 38px;
  cursor: pointer;
}
.toolBtn {
  display: inline-block;
  width: 36px;
  height: 36px;
}
.toolBtn:hover {
  background: #e7f3ff;
}
#helpContainer {
  text-align: left;
  padding-left: 12px;
  padding-right: 12px;
  line-height: 32px;
}
</style>