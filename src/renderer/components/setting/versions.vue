<template>
  <div class="container">
    <div class="block" :key="index" v-for="(item,index) in versions">
      <div class="title">
        <div class="dot"></div>
        <div class="titleText">V {{item.v}}</div>
      </div>
      <div v-html="item.i.replace(/\n/gi,'<br />')" class="content"></div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      versions: [
        {
          v: "6.4.5",
          i: `解决删除文章、火花时，底部左下角相关统计数字不会更新的BUG；
          `
        },
        {
          v: "6.4.4",
          i: `electron版本更新至6.0.1（底层为chrome 76）；
    移除原生图片处理组件sharp，换为更轻量的compressor.js，安装包体积更小；
    新增数据导入导出功能（设置->数据管理）；
    修复可以重复添加相同的标签的BUG；
    开始重构部分代码，增强用户体验，减少BUG；`
        },
        {
          v: "6.4.3",
          i: `为了和mac版保持版本号的一致，跳了一个小版本；
    修复部分文章和脑图无法显示的BUG，
    修复脑图某些节点关闭后，再次打开该脑图布局错乱的BUG；`
        },
        {
          v: "6.4.1",
          i: `支持搜狐号；
    重构最近打开的记录，增加人性化提示，
    html编辑器图片拖动改变大小时高度有限制的问题；
    脑图节点间的曲线更为平滑圆润；
    增加官网链接，修复mac下火花样式上的一个小bug及其他问题；`
        },
        {
          v: "6.4.0*",
          i: `不兼容6.2.14及以前的版本，丢掉历史包袱，速度更快；
    完成思维导图的基本功能；
    markdown编辑的知识同步到“教你啊”时，以HTML的形式同步；
    electron版本更新至5.0.2，sharp版本更新至0.22.1，更稳定，更安全，更快速；
    修改文章标签的操作方式；其他优化工作；`
        },
        {
          v: "6.3.8",
          i: `修复文章标签输入不合法时，错误提示的BUG；
    增加脑图、日程两个菜单；
    支持大鱼号自媒体平台；`
        },
        {
          v: "6.3.7",
          i: `支持爱奇艺自媒体平台；
    解决头条号文章发布后窗口无法关闭的BUG；
    删除wordpress的支持，抽象版本升级信息，增加系统说明内容；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.6",
          i: `移除标签字数长度的限制；
    允许删除火花的标签；
    系统版本升级信息本地化；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.5",
          i: `移除浏览器插件中C++代码；
    抽象火花儿标签列表组件；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.4",
          i: `修复知乎发布BUG；
    修复Ctrl+Z撤销时会撤销多步的BUG；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.3",
          i: `抽象标签删除逻辑，文章删除标签，火花删除标签共用一套逻辑；
    抽象主线程与渲染线程的通信机制；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.2",
          i: `增加“思想火花”模块；
    浏览器插件支持采集简短的文字到“思想火花”模块；
    修改系统首页布局，移除顶部Tab页，增加左侧menu bar；
    修复系统菜单BUG及其他BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.1",
          i: `增加浏览器插件，浏览器插件支持从网页采集知识；
    增加版本号及版本升级信息（设置->版本更新）；
    修复一些小BUG，代码优化，性能优化；`
        },
        {
          v: "6.3.0*",
          i: `不兼容6.2.6以前的版本，丢掉历史包袱，速度更快；
    发布知识后同步一份到“教你啊”,为将来的多端同步，知识社交奠定基础，允许用户取消同步；
    美化系统设置页面，分多标签页；
    提示用户登录，共享“教你啊”的账号体系，使用便捷的微信扫码登录方式；`
        },
        {
          v: "6.2.14",
          i: `支持markdown编辑器；
    支持发三大自媒体平台：头条号、企鹅号、百家号；且支持豆瓣；
    修复一系列易用性问题，提升用户操作体验；
    加入新版本静默下载，自动升级功能；`
        },
        {
          v: "6.2.13",
          i: `老版本暂不提供升级信息记录；`
        }
      ]
    };
  }
};
</script>
<style scoped>
.container {
  margin-top: 16px;
  margin-left: 26px;
}

.block:last-child {
  border-left: none !important;
}

.block {
  border-left: 1px solid #1787fb;
}

.title {
  position: relative;
  left: -5px;
  top: -12px;
}

.dot {
  background: #1787fb;
  width: 10px;
  height: 10px;
  border-radius: 5px;
  display: inline-block;
}

.titleText {
  color: #1787fb;
  font-weight: bolder;
  display: inline-block;
  padding-left: 8px;
}

.content {
  margin-top: -8px;
  padding-left: 16px;
  padding-bottom: 18px;
  color: #555;
}
</style>