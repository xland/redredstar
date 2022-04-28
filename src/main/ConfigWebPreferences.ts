import type { WebPreferences } from 'electron'
export class ConfigWebPreferences implements WebPreferences {
  preload?
  nodeIntegration = true
  devTools = true
  webSecurity = false
  nodeIntegrationInSubFrames = true
  nodeIntegrationInWorker = true
  worldSafeExecuteJavaScript = true
  contextIsolation = false
  allowRunningInsecureContent = true
  center = true
  webgl = false
  disableHtmlFullscreenWindowResize = true
  enableWebSQL = false
  spellcheck = false
  webviewTag = true
}
