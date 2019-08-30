<template>
  <div v-if="setting" class="setting view">
    <div class="leftMenu">
      <div
        @click="menuSelect(index)"
        :class="['item',index == menuIndex?'selected':'']"
        :key="index"
        v-for="(item,index) in menuItems"
      >{{item}}</div>
      <div class="other"></div>
    </div>
    <div v-show="menuIndex == 0" class="content">
      <div class="formItem">
        <div>自动保存文章的时间间隔：</div>
        <div>
          <input
            @change="autoSaveIntervalSeconds"
            v-model="setting.autosave_interval"
            type="number"
          />
        </div>
        <div>秒</div>
      </div>
      <div class="formItem">
        <div>文章内图片长超过</div>
        <div>
          <input @change="compressHeight" v-model="setting.img_h" type="number" />
        </div>
        <div>px，且宽超过</div>
        <div>
          <input @change="compressWidth" v-model="setting.img_w" type="number" />
        </div>
        <div>px，将启用图片压缩（任一值设置为-1将禁用图片压缩）</div>
      </div>
      <div class="formItem">
        默认编辑器：
        <div @click="setting.editor_type='html'" class="rdBtn">
          <i :class="['iconfont',setting.editor_type=='html'?'icon-xuanzhong':'icon-weixuanzhong']"></i>
          Html
        </div>
        <div @click="setting.editor_type='markdown'" class="rdBtn">
          <i
            :class="['iconfont',setting.editor_type=='markdown'?'icon-xuanzhong':'icon-weixuanzhong']"
          ></i>
          MarkDown
        </div>
      </div>
      <div class="formItem">
        发布文章时，同时发布到 “
        <div @click="gotoJna('https://jiaonia.com')" class="link">教你啊</div>”
        <div @click="setting.jna_sync=!setting.jna_sync" class="rdBtn">
          <i :class="['iconfont',setting.jna_sync?'icon-xuanzhong':'icon-weixuanzhong']"></i>
        </div>（未来PC端与移动端同步的基础）
      </div>
      <div class="formItem">
        <div @click="save" class="btn" style="margin-left: 0px;">保存系统设置</div>
      </div>
    </div>
    <div v-if="menuIndex == 1" class="content">
      <user />
    </div>
    <div v-show="menuIndex == 2" class="content">
      <help />
    </div>
    <div v-show="menuIndex == 3" class="content">
      <versions />
    </div>
    <div v-show="menuIndex == 4" class="content">
      <dataManage />
    </div>
  </div>
</template>
<script>
//todo:删除文章或者火花时，不要提示我
import swal from "sweetalert";
import versions from "../components/setting/versions";
import help from "../components/setting/help";
import user from "../components/setting/user";
import dataManage from "../components/setting/dataManage";
export default {
  components: {
    versions,
    help,
    user,
    dataManage
  },
  data() {
    return {
      setting: null,
      menuIndex: 0,
      menuItems: ["系统设置", "用户信息", "系统说明", "版本更新", "数据管理"]
    };
  },
  async mounted() {
    this.setting = await this.db("settings")
      .select("*")
      .first();
  },
  methods: {
    menuSelect(index) {
      this.menuIndex = index;
    },
    async save() {
      await this.db("settings")
        .update(this.setting)
        .where("id", this.setting.id);
      await this.alert("保存成功，将刷新应用", "success");
      window.location.reload();
    },
    alert(str, iconType = "error") {
      return swal({
        icon: iconType,
        text: str
      });
    },
    autoSaveIntervalSeconds() {
      if (this.setting.autosave_interval < 3) {
        this.setting.autosave_interval = 3;
        this.alert("自动保存时间间隔不能小于3秒");
      }
    },
    compressWidth() {
      if (this.setting.img_w < 500 && this.setting.img_w > 0) {
        this.setting.img_w = 500;
        this.alert("最小宽度不能小于500");
      }
    },
    compressHeight() {
      if (this.setting.img_h < 300 && this.setting.img_h > 0) {
        this.setting.img_h = 300;
        this.alert("最小高度不能小于300");
      }
    }
  }
};
</script>
<style>
.setting {
  display: flex;
  background: #fff;
  height: calc(100% - 8px);
  margin: 0px 8px 0px 8px;
  box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
  border-radius: 3px;
}

.content {
  flex: 1;
  color: #666;
  line-height: 32px;
  height: 100%;
  overflow-y: auto;
}

.leftMenu {
  width: 130px;
  line-height: 46px;
  text-align: center;
  display: flex;
  flex-direction: column;
}

.leftMenu .item {
  height: 46px;
  cursor: pointer;
  border-right: 2px solid #eee;
}

.leftMenu .item:hover {
  background: #f9f9f9;
}

.leftMenu .selected {
  background: #f0faff !important;
  border-right: 2px solid #1787fb;
}

.leftMenu .other {
  flex: 1;
  border-right: 2px solid #eee;
}

.formItem {
  padding: 0px 18px 0px 18px;
  height: 42px;
  line-height: 42px;
  color: #363636;
}

.link {
  color: #1787fb;
  text-decoration: underline;
  cursor: pointer;
}

.formItem input {
  border: 1px solid #eee;
  border-radius: 3px;
  line-height: 22px;
  height: 22px;
  outline: none;
  text-align: center;
  width: 50px;
  font-size: 12px;
}

.formItem div {
  display: inline-block;
}

.formItem div:last-child {
  padding-left: 8px;
}

.rdBtn {
  cursor: pointer;
}

.icon-xuanzhong {
  color: #1787fb !important;
}

.icon-weixuanzhong {
  color: #aaa !important;
}
</style>