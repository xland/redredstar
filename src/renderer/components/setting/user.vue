<template>
  <div style="padding: 8px 18px 8px 18px;" v-if="userInfo != null">
    <div style="color: #888">以下信息是您在“教你啊”网站的用户信息：</div>
    <div style="display: flex">
      <div style="width: 260px">
        <div class="formItem">昵称：{{userInfo.nick_name}}</div>
        <div class="formItem">邮箱：{{userInfo.email}}</div>
        <div class="formItem">电话：{{userInfo.phone}}</div>
        <div class="formItem">提问数量：{{userInfo.question_num}}</div>
        <div class="formItem">回答数量：{{userInfo.answer_num}}</div>
        <div class="formItem">积分：{{userInfo.score}}</div>
        <div class="formItem">总金额：{{userInfo.total_money}}元</div>
      </div>
      <div style="flex: 1;">
        <img :src="userInfo.avatar" />
      </div>
    </div>
    <div class="formItem" style="padding-left: 0px">
      <div
        @click="gotoJna('https://jiaonia.com/My/Info/')"
        class="btn"
        style="margin-left: 0px;"
      >去“教你啊”修改信息</div>
      <div @click="logout()" class="btn" style="background: #ccc;">退出登录</div>
    </div>
  </div>
</template>
<script>
    var electron = require('electron');
    //todo 刚完成扫码登录之后，好像这个画面出现不了
export default {
  data() {
    return {
      userInfo: null
    };
  },
  mounted() {
    if (!this.$root.jnaToken) {
      this.bus.$emit("login");
      this.menuIndex = 0;
    } else {
      let self = this;
      let fd = new FormData();
      fd.append("token", this.$root.jnaToken);
      window.xxmPost("https://jiaonia.com/Xxm/GetUserInfoByToken", fd, rt => {
        self.userInfo = JSON.parse(rt).data;
      });
    }
  },
  methods: {
    async logout() {
      this.$root.jnaToken = null;
      this.$root.userInfo = null;
      this.setting.jna_token = null;
      await this.db("settings").update({ jna_token: null });
      this.menuIndex = 0;
    },
    gotoJna(url) {
      electron.remote.shell.openExternal(url);
    }
  }
};
</script>

