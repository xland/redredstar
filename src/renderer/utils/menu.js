const remote = require('electron').remote;
const Menu = remote.Menu;
const application = {
    label: 'Application',
    submenu: [{
            label: '关于系统',
            role: 'about',
        },
        {
            type: 'separator',
        },
        {
            label: '退出',
            accelerator: 'Command+Q',
            click: () => {
                require('electron').app.quit();
            },
        },
    ],
};

const edit = {
    label: '编辑',
    submenu: [{
            label: '撤销',
            accelerator: 'CmdOrCtrl+Z',
            role: 'undo',
        },
        {
            label: '重做',
            accelerator: 'Shift+CmdOrCtrl+Z',
            role: 'redo',
        },
        {
            type: 'separator',
        },
        {
            label: '剪切',
            accelerator: 'CmdOrCtrl+X',
            role: 'cut',
        },
        {
            label: '复制',
            accelerator: 'CmdOrCtrl+C',
            role: 'copy',
        },
        {
            label: '黏贴',
            accelerator: 'CmdOrCtrl+V',
            role: 'paste',
        },
        {
            label: '全选',
            accelerator: 'CmdOrCtrl+A',
            role: 'selectAll',
        },
    ],
};

if (process.platform == 'darwin') {
    Menu.setApplicationMenu(Menu.buildFromTemplate([application, edit]));
}