const { app, BrowserWindow, Menu } = require('electron');

if (require('electron-squirrel-startup')) return app.quit();

const path = require('path');

const template = [
  {
    label: app.name,
    submenu: [
      {
        label: 'New Game',
        submenu: [
          {
            label: 'No agent',
            click: _ => createWindow('[false, false]'),
            accelerator: process.platform === 'darwin' ? 'Cmd+Shift+H' : 'Alt+Shift+H',
            type: 'radio',
            checked: true
          },
          {
            label: 'Human first',
            click: _ => createWindow('[false, true]'),
            accelerator: process.platform === 'darwin' ? 'Cmd+H' : 'Alt+H',
            type: 'radio',
          },
          {
            label: 'Agent first',
            click: _ => createWindow('[true, false]'),
            accelerator: process.platform === 'darwin' ? 'Cmd+A' : 'Alt+A',
            type: 'radio',
          },
          {
            label: 'No human',
            click: _ => createWindow('[true, true]'),
            accelerator: process.platform === 'darwin' ? 'Cmd+Shift+A' : 'Alt+Shift+A',
            type: 'radio',
          },
        ]
      },
      { role: 'quit' },
    ]
  }
];

const menu = Menu.buildFromTemplate(template);
Menu.setApplicationMenu(menu);

var lastWin = null;

function createWindow(isAgent) {
  const win = new BrowserWindow({
    resizable: false,
    fullscreenable: false,
    width: 600,
    height: 600,
    useContentSize: true,
    backgroundColor: 'burlywood',
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      additionalArguments: [isAgent],
      nodeIntegrationInWorker: true
    }
  });

  // win.webContents.openDevTools();
  win.loadFile('index.html');

  if (lastWin !== null) {
    lastWin.close();
  }

  lastWin = win;
}

app.whenReady().then(() => {
  createWindow('[false, false]');
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit();
})
