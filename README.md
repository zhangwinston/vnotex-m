# VNote
![CI-Windows](https://github.com/vnotex/vnote/workflows/CI-Windows/badge.svg) ![CI-Linux](https://github.com/vnotex/vnote/workflows/CI-Linux/badge.svg) ![CI-MacOS](https://github.com/vnotex/vnote/workflows/CI-MacOS/badge.svg)

个人定制版本主要改动：
1. 编辑器的行间距可配置调整
   用户配置文件设置，文本行间距，缺省为0.5；代码行间距，缺省为0.2。
   自行编译需要结合配套的vtextedit-m的定制化修改。
2. 界面图标和界面的微调
   基于开源免费svg调整了界面图标。
3. 当前tab页的文件名粗体显示
4. 防盗链网页的图片下载功能
   在文档头部手工填写referer信息，例如：@@https://cloud.tencent.com/，即可下载相关图片。
5. syntax-highlighting组件升级
   从5.62升级到5.87，支持的高亮语法类型数量倒是有增加，从原来的311个，增加到333个；另外就是高亮的主题数量也有所增加，从原来的7个增加到24个。
   自行编译需要结合对应的syntax-highlighting组件。

[简体中文](README_zh_CN.md)

A pleasant note-taking platform.

For more information, please visit [**VNote's Home Page**](https://vnotex.github.io/vnote) or [Home Page on Gitee](https://tamlok.gitee.io/vnote).

![VNote](pics/vnote.png)

## Description
**VNote** is a Qt-based, free and open source note-taking application, focusing on Markdown now. VNote is designed to provide a pleasant note-taking platform with excellent editing experience.

VNote is **NOT** just a simple editor for Markdown. By providing notes management, VNote makes taking notes in Markdown simpler. In the future, VNote will support more formats besides Markdown.

Utilizing Qt, VNote could run on **Linux**, **Windows**, and **macOS**.

![Main](pics/main.png)

## Downloads
Continuous builds on `master` branch could be found at the [Continuous Build](https://github.com/vnotex/vnote/releases/tag/continuous-build) release.

Latest stable builds could be found at the [latest release](https://github.com/vnotex/vnote/releases/latest). Alternative download services are available:

* [Tianyi Netdisk](https://cloud.189.cn/t/Av67NvmEJVBv)
* [Baidu Netdisk](https://pan.baidu.com/s/1Fou1flmBsQUQ8Qs9V_M6Aw) with the code `note`

## Supports
* [GitHub Issues](https://github.com/vnotex/vnote/issues);
* Email: `tamlokveer at gmail.com`;
* [Slack](https://join.slack.com/t/vnote/shared_invite/enQtNDg2MzY0NDg3NzI4LTVhMzBlOTY0YzVhMmQyMTFmZDdhY2M3MDQxYTBjOTA2Y2IxOGRiZjg2NzdhMjkzYmUyY2VkMWJlZTNhMTQyODU);
* [Telegram](https://t.me/vnotex);
* WeChat Public Account: vnotex;

## Donate
You could help VNote's development in many ways.

* Keep monitoring VNote and sending feedback for improvement.
* Spread and promote VNote to your friends. Popularity is a strong power to drive developers.
* Participate in the development of VNote and send [Pull Request](https://github.com/vnotex/vnote/pulls) to make VNote perfect.
* Last, really appreciate your donations to VNote if VNote does help.

**PayPal**: [PayPal.Me/vnotemd](https://www.paypal.me/vnotemd)

**Alipay**: `tamlokveer@gmail.com`

<img src="pics/alipay.png" width="256px" height="256px" />

**WeChat**

<img src="pics/wechat_pay.png" width="256px" height="256px" />

Thank [users who donated to VNote](https://github.com/vnotex/vnote/wiki/Donate-List)!

## License
VNote is licensed under [GNU LGPLv3](https://opensource.org/licenses/LGPL-3.0). Code base of VNote could be used freely by VNoteX.
