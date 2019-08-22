<template>
  <div>
    <div class="top"></div>
    <topMenu></topMenu>
    <titleContent></titleContent>
    <imgSwitch></imgSwitch>
    <publishSites />
    <div id="funcList" class="funcList">
      <div class="gifBox">
        <img src="http://www.xiangxuema.com/doc/search.gif" alt />
      </div>
      <div class="des" style="padding-left: 68px;">
        <div class="title">知识检索</div>
        <div class="desDetail">
          结合标签库与搜索框，提供快速定位知识的能力;
          <br />知识库、脑图库、火花库共享一套标签库;
          <br />标签栏位于知识库列表右侧，其他亦然;
          <br />给知识增加标签时，如果库中已存在该标签，那该标签的引用计数会加一，如果不存在该标签，系统会自动增加该标签到标签库中；
          <br />如果没有任何一个知识、火花、脑图引用一个标签，那么这个标签将被自动删除；
        </div>
      </div>
    </div>
    <div class="funcList">
      <div class="des" style="padding-right: 68px;">
        <div class="title">知识编辑与发布</div>
        <div class="desDetail">
          支持图片复制黏贴到编辑窗口，亦可截屏后黏贴;
          <br />markdown编辑器亦可直接黏贴图片到文章内;
          <br />编辑器内图片支持拖拽改变大小;
          <br />支持Ctrl+S（保存）、Ctrl+F（查找）等快捷键；
          <br />目前文章发布到13+个博客及自媒体平台；
          <br />文章发布不包括任何广告，文章内的“尾巴链接”；
          <br />文章（包括文章内的图片）均保存在本地，随时导出数据；
        </div>
      </div>
      <div class="gifBox">
        <img
          style="
      width: 817px;
      height: 491px;
      margin-left: -7px;
      margin-top: -4px;"
          src="http://www.xiangxuema.com/doc/publish.gif"
        />
      </div>
    </div>
    <div class="funcList">
      <div class="gifBox">
        <img src="http://www.xiangxuema.com/doc/extension_article.gif" alt />
      </div>
      <div class="des" style="padding-left: 68px;">
        <div class="title">浏览器插件</div>
        <div class="desDetail">
          结合浏览器插件，快速收集互联网文章和idea;
          <br />文章采集，只需要点两下，火花采集点一下，神速;
          <br />而且只采集你想要的内容，要你所要;
          <br />浏览器插件也支持快捷键;
          <br />通过浏览器插件采集知识，知识内的图片也会被下载到本地电脑；
          <br />提供科学上网安装与手动安装插件两种安装方式；
        </div>
      </div>
    </div>
    <div id="funcsPermissions" class="textBox">
      <div class="textTitle">功能及责权说明</div>
      <div class="textDesc">
        <ul>
          <li>您通过“想学吗”编辑的知识，以及知识内部的图片、个人设置等数据均保存在本地；</li>
          <li>您的知识，可以自由的发布到“微信公众号”、“博客园”、“开源中国”等十几个知名网站（需拥有相应网站的账号）；</li>
          <li>发布知识时，知识内部的图片也会上传到对应的网站上；</li>
          <li>修改知识后，再次发布该知识，不会导致图片重复上传；</li>
          <li>在文章中删除图片，本地目录中的图片也会被删除，不会留有垃圾数据；</li>
          <li>“想学吗”不会保存您任何博客平台的账号数据；</li>
          <li>文章发布到目标平台不夹带任何“尾巴”；</li>
          <li>“想学吗”浏览器插件包括文章收集功能和思想火花收集功能，可以快速收集互联网的文章或简短的文字；</li>
          <li>“想学吗”浏览器插件收集文章时，也会把文章内的图片下载到本地；</li>
          <li>“想学吗”还提供思维导图的功能，可以快速梳理构建您脑海中的想法</li>
        </ul>
      </div>
      <div
        class="textDesc"
        style="margin-top:18px;text-align:center;"
      >本软件为非盈利、开源软件，任何责任与义务均由使用者承担，与开发者无关；</div>
    </div>
    <div class="footer">
      <div class="footerTop">
        合作伙伴：
        <br />湖南心莱信息科技有限公司 &nbsp;&nbsp;&nbsp;&nbsp;
        苏州盛派网络科技有限公司 &nbsp;&nbsp;&nbsp;&nbsp;
        衡水浩伦科技有限公司 &nbsp;&nbsp;&nbsp;&nbsp;
      </div>
      <div class="footerBottom">
        <div style="text-align:left">杭州软雅科技有限公司 © 2019 All Rights Reserved.</div>
        <div style="text-align:right">浙ICP备07001688号 ya-soft ®</div>
      </div>
    </div>
  </div>
</template><script>
import topMenu from "../components/topMenu";
import titleContent from "../components/titleContent";
import imgSwitch from "../components/imgSwitch";
import publishSites from "../components/publishSites";
export default {
  components: {
    topMenu,
    titleContent,
    imgSwitch,
    publishSites
  },
  data() {
    return {};
  },
  mounted() {},
  methods: {}
};
</script>

<style>
html,
body {
  margin: 0px;
  padding: 0px;
  overflow-x: hidden;
}
</style>
<style lang="scss" scoped>
.footer {
  background: #333;
  color: #888;
  padding-left: 16%;
  padding-right: 16%;
  padding-top: 50px;
  padding-bottom: 50px;
  font-size: 15px;
  .footerTop {
    border-bottom: 1px solid #555;
    text-align: left;
    line-height: 36px;
    padding-bottom: 12px;
  }
  .footerBottom {
    display: flex;
    padding-top: 18px;
    padding-bottom: 12px;
    div{
      flex: 1;
    }
  }
}
.top {
  height: 8000px;
  width: 8000px;
  background: #f3f4f6;
  border-bottom-left-radius: 4000px;
  border-bottom-right-radius: 4000px;
  position: absolute;
  top: -7260px;
  left: calc(50% - 4000px);
  z-index: -1;
}
.textBox {
  padding-top: 80px;
  padding-bottom: 80px;
  .textTitle {
    font-size: 36px;
    font-weight: 600;
    line-height: 60px;
    color: #43474e;
    padding-bottom: 20px;
    padding-bottom: 38px;
  }
  .textDesc {
    background: #f3f4f6;
    border-radius: 8px;
    margin-left: auto;
    margin-right: auto;
    width: 860px;
    text-align: left;
    padding: 26px;
    line-height: 38px;
    font-size: 16px;
  }
}
.funcList {
  display: flex;
  padding-top: 80px;
  padding-bottom: 80px;
  padding-left: 16%;
  padding-right: 16%;
  .gifBox {
    width: 805px;
    height: 480px;
    overflow: hidden;
    border-radius: 4px;
    box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
    img {
      width: 823px;
      height: 491px;
      margin-left: -7px;
      margin-top: -4px;
    }
  }
  .des {
    flex: 1;
    text-align: left;
    .title {
      font-size: 36px;
      font-weight: 600;
      line-height: 60px;
      color: #43474e;
      padding-bottom: 20px;
    }
    .desDetail {
      font-size: 18px;
      color: #717682;
      line-height: 48px;
    }
  }
}
</style>