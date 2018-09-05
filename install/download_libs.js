const os = require('os')
const path = require('path')
const fs = require('fs')
const https = require('https')

const platform = os.platform()
const platformFiles = {
  win32: [
    'https://raw.githubusercontent.com/quickdevel/catboostmodel-libs/master/v0.9.1.1/windows/catboostmodel.dll',
    'https://raw.githubusercontent.com/quickdevel/catboostmodel-libs/master/v0.9.1.1/windows/catboostmodel.lib'
  ],
  linux: [
    'https://raw.githubusercontent.com/quickdevel/catboostmodel-libs/master/v0.9.1.1/linux/libcatboostmodel.so',
    'https://raw.githubusercontent.com/quickdevel/catboostmodel-libs/master/v0.9.1.1/linux/libcatboostmodel.so.1'
  ]
}

if (Object.keys(platformFiles).includes(platform)) {
  const libsPath = path.resolve(__dirname, '../libs')
  const files = platformFiles[platform]

  if (!fs.existsSync(libsPath)) {
    fs.mkdirSync(libsPath)
  }

  files.forEach(file => {
    const fileStream = fs.createWriteStream(path.resolve(libsPath, path.basename(file)))
    console.log(`Downloading ${file}...`)
    https.get(file, response => {
      response.pipe(fileStream)
    })
  })
} else {
  console.log(`Unsupported platform (${platform})`)
}
