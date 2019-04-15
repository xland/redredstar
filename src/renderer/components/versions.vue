<template>
    <div class="container">
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.7
                </div>
            </div>
            <div class="content">
                支持爱奇艺；<br />
                解决头条号文章发布后窗口无法关闭的BUG；<br />
                删除wordpress的支持；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.6
                </div>
            </div>
            <div class="content">
                移除标签字数长度的限制；<br />
                允许删除火花的标签；<br />
                系统版本升级信息本地化；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.5
                </div>
            </div>
            <div class="content">
                移除浏览器插件中C++代码；<br />
                抽象火花儿标签列表组件；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.4
                </div>
            </div>
            <div class="content">
                修复知乎发布BUG；<br />
                修复Ctrl+Z撤销时会撤销多步的BUG；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.3
                </div>
            </div>
            <div class="content">
                抽象标签删除逻辑，文章删除标签，火花删除标签共用一套逻辑；<br />
                抽象主线程与渲染线程的通信机制；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.2
                </div>
            </div>
            <div class="content">
                增加“思想火花”模块；<br />
                浏览器插件支持采集简短的文字到“思想火花”模块；<br />
                修改系统首页布局，移除顶部Tab页，增加左侧menu bar；<br />
                修复系统菜单BUG及其他BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.1
                </div>
            </div>
            <div class="content">
                增加浏览器插件，浏览器插件支持从网页采集知识；<br />
                版本号及版本升级信息；<br />
                修复一些小BUG，代码优化，性能优化；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.3.0 （此版本不兼容早期6.2.3版本）
                </div>
            </div>
            <div class="content">
                不兼容早期6.2.6版本，丢掉历史包袱，速度更快，提供手动升级方式；<br />
                发布知识后同步一份到“教你啊”,为将来的多端同步，知识社交奠定基础，允许用户取消同步；<br />
                美化系统设置页面，分多标签页；<br />
                提示用户登录，共享“教你啊”的账号体系，使用便捷的微信扫码登录方式；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.2.14
                </div>
            </div>
            <div class="content">
                支持markdown编辑器；<br />
                支持发三大自媒体平台：头条号、企鹅号、百家号；<br />
                支持豆瓣；<br />
                修复一系列易用性问题，提升用户操作体验；<br />
                加入新版本静默下载，自动升级功能；<br />
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.2.13
                </div>
            </div>
            <div class="content">
                老版本暂不提供升级信息记录；
            </div>
        </div>
        <div class="block">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    v 6.2.12
                </div>
            </div>
            <div class="content">
                老版本暂不提供升级信息记录......
            </div>
        </div>
    </div>
</template>
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